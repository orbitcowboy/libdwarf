/*
  Copyright 2015-2016 David Anderson. All rights reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2 of the GNU General Public License as
  published by the Free Software Foundation.

  This program is distributed in the hope that it would be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  Further, this software is distributed without any warranty that it is
  free of the rightful claim of any third person regarding infringement
  or the like.  Any license provided herein, whether implied or
  otherwise, applies only to this software file.  Patent licenses, if
  any, provided herein do not apply to combinations of this program with
  other software, or any other product whatsoever.

  You should have received a copy of the GNU General Public License along
  with this program; if not, write the Free Software Foundation, Inc., 51
  Franklin Street - Fifth Floor, Boston MA 02110-1301, USA.
*/

#include "globals.h"
#include "esb.h"
#include "dwarf_tsearch.h"
#include "macrocheck.h"

#define TRUE 1
#define FALSE 0
/*  WARNING: the tree walk functions will, if presented **tree
    when *tree is wanted, simply find nothing. No error,
    just bad results. So when a walk produces nothing
    suspect a code mistake here.
    The basic problem is void* is a terrible way to
    pass in a pointer. But it's how tsearch was defined
    long ago.
*/

void *  macro_check_tree;    /* DWARF5 macros */
void *  macinfo_check_tree; /* DWARF 2,3,4 macros */

static struct Macrocheck_Map_Entry_s * macrocheck_map_insert(
    Dwarf_Unsigned off,
    unsigned prim,unsigned sec, void **map);
static struct Macrocheck_Map_Entry_s * macrocheck_map_find(
    Dwarf_Unsigned offset,
    void **map);
static void macrocheck_map_destroy(void *map);
static Dwarf_Unsigned macro_count_recs(void **base);


static struct Macrocheck_Map_Entry_s *
macrocheck_map_create_entry(Dwarf_Unsigned offset,
    unsigned add_primary, unsigned add_secondary)
{
    struct Macrocheck_Map_Entry_s *mp =
        (struct Macrocheck_Map_Entry_s *)
        calloc(1,sizeof(struct Macrocheck_Map_Entry_s));
    if (!mp) {
        return 0;
    }
    mp->mp_key = offset;
    mp->mp_len = 0;
    mp->mp_printed = 0;
    mp->mp_refcount_primary = add_primary;
    mp->mp_refcount_secondary = add_secondary;
    return mp;
}
static void
macrocheck_map_free_func(void *mx)
{
    struct Macrocheck_Map_Entry_s *m = mx;
    free(m);
}

static int
macrocheck_map_compare_func(const void *l, const void *r)
{
    const struct Macrocheck_Map_Entry_s *ml = l;
    const struct Macrocheck_Map_Entry_s *mr = r;
    if (ml->mp_key < mr->mp_key) {
        return -1;
    }
    if (ml->mp_key > mr->mp_key) {
        return 1;
    }
    return 0;
}

static struct Macrocheck_Map_Entry_s *
macrocheck_map_insert(Dwarf_Unsigned offset,
    unsigned add_prim,unsigned add_sec,void **tree1)
{
    void *retval = 0;
    struct Macrocheck_Map_Entry_s *re = 0;
    struct Macrocheck_Map_Entry_s *e;
    e  = macrocheck_map_create_entry(offset,add_prim,add_sec);
    /*  tsearch records e's contents unless e
        is already present . We must not free it till
        destroy time if it got added to tree1.  */
    retval = dwarf_tsearch(e,tree1, macrocheck_map_compare_func);
    if (retval) {
        re = *(struct Macrocheck_Map_Entry_s **)retval;
        if (re != e) {
            /*  We returned an existing record, e not needed.
                Increment refcounts. */
            re->mp_refcount_primary += add_prim;
            re->mp_refcount_secondary += add_sec;
            macrocheck_map_free_func(e);
        } else {
            /* Record e got added to tree1, do not free record e. */
        }
    }
    return NULL;
}

static struct Macrocheck_Map_Entry_s *
macrocheck_map_find(Dwarf_Unsigned offset,void **tree1)
{
    void *retval = 0;
    struct Macrocheck_Map_Entry_s *re = 0;
    struct Macrocheck_Map_Entry_s *e = 0;

    e = macrocheck_map_create_entry(offset,0,0);
    retval = dwarf_tfind(e,tree1, macrocheck_map_compare_func);
    if (retval) {
        re = *(struct Macrocheck_Map_Entry_s **)retval;
    }
    /*  The one we created here must be deleted, it is dead.
        We look at the returned one instead. */
    macrocheck_map_free_func(e);
    return re;
}

static void
macrocheck_map_destroy(void *map)
{
    /* tdestroy is not part of Posix. */
    dwarf_tdestroy(map,macrocheck_map_free_func);
}


void
add_macro_import(void **base,Dwarf_Bool is_primary,Dwarf_Unsigned offset)
{
    Dwarf_Unsigned prim_count  = 0;
    Dwarf_Unsigned sec_count  = 0;

    if(is_primary) {
        prim_count = 1;
    } else {
        sec_count = 1;
    }
    macrocheck_map_insert(offset,prim_count,sec_count,base);
}
void
add_macro_import_sup(UNUSEDARG void **base,
    UNUSEDARG Dwarf_Unsigned offset)
{
    /* FIXME */
    return;
}
void
add_macro_area_len(void **base, Dwarf_Unsigned offset,
    Dwarf_Unsigned len)
{
    struct Macrocheck_Map_Entry_s *re = 0;

    re = macrocheck_map_find(offset,base);
    if (re) {
        re->mp_len = len;
    }
}

static Dwarf_Unsigned reccount = 0;

static void
macro_walk_count_recs(UNUSEDARG const void *nodep,
    const DW_VISIT which,
    UNUSEDARG const int depth)
{
    if (which == dwarf_postorder || which == dwarf_endorder) {
        return;
    }
    reccount += 1;
}
static Dwarf_Unsigned
macro_count_recs(void **base)
{
    reccount = 0;
    dwarf_twalk(*base,macro_walk_count_recs);
    return reccount;
}

static Dwarf_Unsigned lowestoff = 0;
static Dwarf_Bool lowestfound = FALSE;
static void
macro_walk_find_lowest(const void *nodep,const DW_VISIT  which,
    UNUSEDARG const int  depth)
{
    struct Macrocheck_Map_Entry_s * re =
        *(struct Macrocheck_Map_Entry_s**)nodep;

    if (which == dwarf_postorder || which == dwarf_endorder) {
        return;
    }
    if (!re->mp_printed) {
        if (lowestfound) {
            if (lowestoff > re->mp_key) {
                lowestoff = re->mp_key;
            }
        } else {
            lowestfound = TRUE;
            lowestoff = re->mp_key;
        }
    }
}

int
get_next_unprinted_macro_offset(void **tree, Dwarf_Unsigned * off)
{
    lowestfound = FALSE;
    lowestoff = 0;

    /*  This walks the tree to find one entry.
        Which could get slow if the tree has lots of entries. */
    dwarf_twalk(*tree,macro_walk_find_lowest);
    if (!lowestfound) {
        return DW_DLV_NO_ENTRY;
    }
    *off = lowestoff;
    return DW_DLV_OK;
}

void
mark_macro_offset_printed(void **base, Dwarf_Unsigned offset)
{
    struct Macrocheck_Map_Entry_s *re = 0;

    re = macrocheck_map_find(offset,base);
    if (re) {
        re->mp_printed = TRUE;
    }
}


static struct Macrocheck_Map_Entry_s **mac_as_array = 0;
static unsigned mac_as_array_next = 0;
static void
macro_walk_to_array(const void *nodep,const DW_VISIT  which,
    UNUSEDARG const int  depth)
{
    struct Macrocheck_Map_Entry_s * re =
        *(struct Macrocheck_Map_Entry_s**)nodep;

    if (which == dwarf_postorder || which == dwarf_endorder) {
        return;
    }
    mac_as_array[mac_as_array_next] = re;
    mac_as_array_next++;
}

static int
qsort_compare(const void *lin, const void *rin)
{
    const struct Macrocheck_Map_Entry_s *l =
        *(const struct Macrocheck_Map_Entry_s **)lin;
    const struct Macrocheck_Map_Entry_s *r =
        *(const struct Macrocheck_Map_Entry_s **)rin;
    if (l->mp_key < r->mp_key) {
        return -1;
    }
    if (l->mp_key > r->mp_key) {
        return 1;
    }
    if (l->mp_len < r->mp_len) {
        return -1;
    }
    if (l->mp_len > r->mp_len) {
        return 1;
    }
    return 0;
}

void
print_macro_statistics(const char *name,void **tsbase,
    Dwarf_Unsigned section_size)
{
    Dwarf_Unsigned count = 0;
    Dwarf_Unsigned lowest = -1ll;
    Dwarf_Unsigned highest = 0;
    Dwarf_Unsigned lastend = 0;
    Dwarf_Unsigned laststart = 0;
    Dwarf_Unsigned internalgap = 0;
    Dwarf_Unsigned wholegap = 0;
    Dwarf_Unsigned i = 0;

    if(! *tsbase) {
        return;
    }
    count = macro_count_recs(tsbase);
    if (count < 1) {
        return;
    }
    free(mac_as_array);
    mac_as_array = 0;
    mac_as_array_next = 0;
    mac_as_array = (struct Macrocheck_Map_Entry_s **)calloc(count,
        sizeof(struct Macrocheck_Map_Entry_s *));
    if(!mac_as_array) {
        printf("  Macro checking ERROR %s: "
            "unable to allocate %" DW_PR_DUu "pointers\n",
            name,
            count);
        return;
    }
    dwarf_twalk(*tsbase,macro_walk_to_array);
    printf("  Macro unit count %s: %" DW_PR_DUu "\n",name,count);
    qsort(mac_as_array,
        count,sizeof(struct Macrocheck_Map_Entry_s *),
        qsort_compare);
    for (i = 0; i < count ; ++i) {
        Dwarf_Unsigned end = 0;
        struct Macrocheck_Map_Entry_s *r = mac_as_array[i];
#if 0
        printf("debugging: i %u off 0x%x len 0x%x printed? %u "
            " ref prim: %u  sec: %u\n",
            (unsigned)i,
            (unsigned)r->mp_key,
            (unsigned)r->mp_len,
            (unsigned)r->mp_printed,
            (unsigned)r->mp_refcount_primary,
            (unsigned)r->mp_refcount_secondary);
#endif
        if (r->mp_key < lowest) {
            lowest = r->mp_key;
        }
        end = r->mp_key + r->mp_len;
        if (end > highest) {
            highest = end;
        }
        if (r->mp_refcount_primary > 1) {
            printf(" ERROR: For offset 0x%" DW_PR_XZEROS DW_PR_DUx
                " there is a primary count of "
                "0x%"  DW_PR_XZEROS DW_PR_DUx "\n",
                r->mp_key,
                r->mp_refcount_primary);
        }
        if (r->mp_refcount_primary && r->mp_refcount_secondary) {
            printf(" ERROR: For offset 0x%" DW_PR_XZEROS DW_PR_DUx
                " there is a nonzero primary count of "
                "0x%"  DW_PR_XZEROS DW_PR_DUx
                " with a secondary count of "
                "0x%"  DW_PR_XZEROS DW_PR_DUx
                "\n",
                r->mp_key,
                r->mp_refcount_primary,
                r->mp_refcount_secondary);
        }
    }
    lastend =
        mac_as_array[0]->mp_key +
        mac_as_array[0]->mp_len;
    laststart = mac_as_array[0]->mp_key;
    printf("  Macro Offsets start at 0x%" DW_PR_XZEROS DW_PR_DUx
        " and end at 0x%" DW_PR_XZEROS DW_PR_DUx "\n",
        lowest, highest);
    for (i = 1; i < count ; ++i) {
        struct Macrocheck_Map_Entry_s *r = mac_as_array[i];
#if 0
        printf("debugging i %u off 0x%x len 0x%x\n",
            (unsigned)i,
            (unsigned)r->mp_key,
            (unsigned)r->mp_len);
#endif
        if (r->mp_key > lastend) {
            internalgap += (r->mp_key - lastend);
        } else if (r->mp_key < lastend) {
            /* crazy overlap */
            printf(" ERROR: For offset 0x%" DW_PR_XZEROS DW_PR_DUx
                " there is a crazy overlap with the previous end offset of "
                "0x%"  DW_PR_XZEROS DW_PR_DUx
                " (previous start offset of 0x%"  DW_PR_XZEROS DW_PR_DUx ")"
                "\n",r->mp_key,lastend,laststart);
        }
        laststart = r->mp_key;
        lastend   = laststart + r->mp_len;
    }
    /*  wholegap is a) starting offset > 0 and b)
        space after used area before end of section. */
    wholegap = mac_as_array[0]->mp_key + internalgap;
    if (lastend > section_size) {
        /* Something seriously wrong */
        printf(" ERROR: For offset 0x%" DW_PR_XZEROS DW_PR_DUx
            " there is an overlap with the end of section "
            "0x%"  DW_PR_XZEROS DW_PR_DUx
            "\n",laststart,lastend);
    } else {
        wholegap += (section_size - lastend);
    }
    if(wholegap) {
        printf("  Macro Offsets internal unused space: "
            "0x%" DW_PR_XZEROS DW_PR_DUx
            "\n",
            internalgap);
        printf("  Macro Offsets total    unused space: "
            "0x%" DW_PR_XZEROS DW_PR_DUx
            "\n",
            wholegap);
    }
    free (mac_as_array);
    mac_as_array = 0;
    mac_as_array_next = 0;
}

void
clear_macro_statistics(void **tsbase)
{
    if(! *tsbase) {
        return;
    }
    macrocheck_map_destroy(*tsbase);
    *tsbase = 0;
}


#ifdef SELFTEST

int main()
{
    void * base = 0;
    Dwarf_Unsigned count = 0;

    /* Test 1 */
    add_macro_import(&base,TRUE,200);
    count = macro_count_recs(&base);
    if (count != 1) {
        printf("FAIL: expect count 1, got %" DW_PR_DUu "\n",count);
    }
    print_macro_statistics("test1",&base,2000);
    add_macro_area_len(&base,200,100);
    add_macro_import(&base,FALSE,350);
    add_macro_area_len(&base,350,100);
    count = macro_count_recs(&base);
    if (count != 2) {
        printf("FAIL: expect count 2, got %" DW_PR_DUu "\n",count);
    }
    print_macro_statistics("test 2",&base,2000);
    clear_macro_statistics(&base);

    /* Test two */
    add_macro_import(&base,TRUE,0);
    add_macro_area_len(&base,0,1000);

    add_macro_import(&base,FALSE,2000);
    add_macro_area_len(&base,2000,100);
    mark_macro_offset_printed(&base,2000);

    add_macro_import(&base,FALSE,1000);
    add_macro_area_len(&base,1000,900);

    add_macro_import(&base,FALSE,1000);
    add_macro_area_len(&base,1000,900);

    count = macro_count_recs(&base);
    if (count != 3) {
        printf("FAIL: expect count 3, got %" DW_PR_DUu "\n",count);
    }

    print_macro_statistics("test 3",&base,2000);
    clear_macro_statistics(&base);
    /* Test three */
    add_macro_import(&base,TRUE,50);
    add_macro_import(&base,TRUE,50);
    add_macro_area_len(&base,50,50);
    add_macro_import(&base,FALSE,200);
    add_macro_import(&base,FALSE,50);
    add_macro_import(&base,FALSE,60);
    add_macro_area_len(&base,60,10);

    print_macro_statistics("test 4",&base,2000);
    clear_macro_statistics(&base);
    return 0;
}
#endif /* SELFTEST */
