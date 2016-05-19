/*

  Copyright (C) 2000-2005 Silicon Graphics, Inc. All Rights Reserved.
  Portions Copyright (C) 2008-2016 David Anderson.  All Rights Reserved.

  This program is free software; you can redistribute it and/or modify it
  under the terms of version 2.1 of the GNU Lesser General Public License
  as published by the Free Software Foundation.

  This program is distributed in the hope that it would be useful, but
  WITHOUT ANY WARRANTY; without even the implied warranty of
  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.

  Further, this software is distributed without any warranty that it is
  free of the rightful claim of any third person regarding infringement
  or the like.  Any license provided herein, whether implied or
  otherwise, applies only to this software file.  Patent licenses, if
  any, provided herein do not apply to combinations of this program with
  other software, or any other product whatsoever.

  You should have received a copy of the GNU Lesser General Public
  License along with this program; if not, write the Free Software
  Foundation, Inc., 51 Franklin Street - Fifth Floor, Boston MA 02110-1301,
  USA.

*/

#ifdef TESTING
#include "config.h"
#include "dwarf_incl.h"
#include <stdio.h>
#include <stdlib.h>  /* For exit() declaration etc. */
#endif  /* TESTING */

/* Array to hold string representation of errors. Any time a
   define is added to the list in libdwarf.h, a string should be
   added to this Array

   Errors in the list (missing a comma, for example)
   happen too often. Making this a separate little file
   simplfies testing for missing-commas/extra-strings.

   String count should match DW_DLE_LAST+1
*/

const char *_dwarf_errmsgs[] = {
    "No error (0)\n",
    "DW_DLE_VMM (1) dwarf format/library version mismatch",
    "DW_DLE_MAP (2) memory map failure",
    "DW_DLE_LEE (3) libelf error",
    "DW_DLE_NDS (4) no debug section",
    "DW_DLE_NLS (5) no line section ",
    "DW_DLE_ID (6) invalid descriptor for query ",
    "DW_DLE_IOF (7) I/O failure ",
    "DW_DLE_MAF (8) memory allocation failure ",
    "DW_DLE_IA (9) invalid argument ",
    "DW_DLE_MDE (10) mangled debugging entry ",
    "DW_DLE_MLE (11) mangled line number entry ",
    "DW_DLE_FNO (12) file not open ",
    "DW_DLE_FNR (13) file not a regular file ",
    "DW_DLE_FWA (14) file open with wrong access ",
    "DW_DLE_NOB (15) not an object file ",
    "DW_DLE_MOF (16) mangled object file header ",
    "DW_DLE_EOLL (17) end of location list entries ",
    "DW_DLE_NOLL (18) no location list section ",
    "DW_DLE_BADOFF (19) Invalid offset ",
    "DW_DLE_EOS (20) end of section  ",
    "DW_DLE_ATRUNC (21) abbreviations section appears truncated",
    "DW_DLE_BADBITC (22)  Address size passed to dwarf bad",

    "DW_DLE_DBG_ALLOC (23) Unable to malloc a Dwarf_Debug structure",
    "DW_DLE_FSTAT_ERROR (24) The file fd passed to dwarf_init "
        "cannot be fstat()ed",
    "DW_DLE_FSTAT_MODE_ERROR (25) The file mode bits do not "
        "indicate that the file being opened via "
        "dwarf_init() is a normal file",
    "DW_DLE_INIT_ACCESS_WRONG (26) A call to dwarf_init had an "
        "access of other than DW_DLC_READ",
    "DW_DLE_ELF_BEGIN_ERROR (27) a call to "
        "elf_begin(... ELF_C_READ_MMAP... ) failed",
    "DW_DLE_ELF_GETEHDR_ERROR (28) a call to "
        "elf32_getehdr() or elf64_getehdr() failed",
    "DW_DLE_ELF_GETSHDR_ERROR (29) a call to "
        "elf32_getshdr() or elf64_getshdr() failed",
    "DW_DLE_ELF_STRPTR_ERROR (30) a call to "
        "elf_strptr() failed trying to get a section name",
    "DW_DLE_DEBUG_INFO_DUPLICATE (31)  Only one .debug_info  "
        "section is allowed",
    "DW_DLE_DEBUG_INFO_NULL (32) .debug_info section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_ABBREV_DUPLICATE (33) Only one .debug_abbrev  "
        "section is allowed",
    "DW_DLE_DEBUG_ABBREV_NULL (34) .debug_abbrev section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_ARANGES_DUPLICATE (35) Only one .debug_aranges  "
        "section is allowed",
    "DW_DLE_DEBUG_ARANGES_NULL (36) .debug_aranges section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_LINE_DUPLICATE (37) Only one .debug_line  "
        "section is allowed",
    "DW_DLE_DEBUG_LINE_NULL (38) .debug_line section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_LOC_DUPLICATE (39) Only one .debug_loc  "
        "section is allowed",
    "DW_DLE_DEBUG_LOC_NULL (40) .debug_loc section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_MACINFO_DUPLICATE (41) Only one .debug_macinfo  "
        "section is allowed",
    "DW_DLE_DEBUG_MACINFO_NULL (42) .debug_macinfo section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_PUBNAMES_DUPLICATE (43) Only one .debug_pubnames  "
        "section is allowed",
    "DW_DLE_DEBUG_PUBNAMES_NULL (44) .debug_pubnames section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_STR_DUPLICATE (45)  Only one .debug_str  "
        "section is allowed",
    "DW_DLE_DEBUG_STR_NULL (46) .debug_str section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_CU_LENGTH_ERROR (47)",
    "DW_DLE_VERSION_STAMP_ERROR (48)",
    "DW_DLE_ABBREV_OFFSET_ERROR (49)",
    "DW_DLE_ADDRESS_SIZE_ERROR (50) Either too large or zero",
    "DW_DLE_DEBUG_INFO_PTR_NULL (51)",
    "DW_DLE_DIE_NULL (52)",
    "DW_DLE_STRING_OFFSET_BAD (53)",
    "DW_DLE_DEBUG_LINE_LENGTH_BAD (54)",
    "DW_DLE_LINE_PROLOG_LENGTH_BAD (55)",
    "DW_DLE_LINE_NUM_OPERANDS_BAD (56)",
    "DW_DLE_LINE_SET_ADDR_ERROR (57)",
    "DW_DLE_LINE_EXT_OPCODE_BAD (58)",
    "DW_DLE_DWARF_LINE_NULL (59)",
    "DW_DLE_INCL_DIR_NUM_BAD (60)",
    "DW_DLE_LINE_FILE_NUM_BAD (61)",
    "DW_DLE_ALLOC_FAIL (62)",
    "DW_DLE_NO_CALLBACK_FUNC (63)",
    "DW_DLE_SECT_ALLOC (64)",
    "DW_DLE_FILE_ENTRY_ALLOC (65)",
    "DW_DLE_LINE_ALLOC (66)",
    "DW_DLE_FPGM_ALLOC (67)",
    "DW_DLE_INCDIR_ALLOC (68)",
    "DW_DLE_STRING_ALLOC (69)",
    "DW_DLE_CHUNK_ALLOC (70)",
    "DW_DLE_BYTEOFF_ERR (71)",
    "DW_DLE_CIE_ALLOC (72)",
    "DW_DLE_FDE_ALLOC (73)",
    "DW_DLE_REGNO_OVFL (74)",
    "DW_DLE_CIE_OFFS_ALLOC (75)",
    "DW_DLE_WRONG_ADDRESS (76)",
    "DW_DLE_EXTRA_NEIGHBORS (77)",
    "DW_DLE_WRONG_TAG (78)",
    "DW_DLE_DIE_ALLOC (79)",
    "DW_DLE_PARENT_EXISTS (80)",
    "DW_DLE_DBG_NULL (81)",
    "DW_DLE_DEBUGLINE_ERROR (82)",
    "DW_DLE_DEBUGFRAME_ERROR (83)",
    "DW_DLE_DEBUGINFO_ERROR (84)",
    "DW_DLE_ATTR_ALLOC (85)",
    "DW_DLE_ABBREV_ALLOC (86)",
    "DW_DLE_OFFSET_UFLW (87)",
    "DW_DLE_ELF_SECT_ERR (88)",
    "DW_DLE_DEBUG_FRAME_LENGTH_BAD (89)",
    "DW_DLE_FRAME_VERSION_BAD (90)",
    "DW_DLE_CIE_RET_ADDR_REG_ERROR (91)",
    "DW_DLE_FDE_NULL (92)",
    "DW_DLE_FDE_DBG_NULL (93)",
    "DW_DLE_CIE_NULL (94)",
    "DW_DLE_CIE_DBG_NULL (95)",
    "DW_DLE_FRAME_TABLE_COL_BAD (96)",
    "DW_DLE_PC_NOT_IN_FDE_RANGE (97)",
    "DW_DLE_CIE_INSTR_EXEC_ERROR (98)",
    "DW_DLE_FRAME_INSTR_EXEC_ERROR (99)",
    "DW_DLE_FDE_PTR_NULL (100)",
    "DW_DLE_RET_OP_LIST_NULL (101)",
    "DW_DLE_LINE_CONTEXT_NULL (102)",
    "DW_DLE_DBG_NO_CU_CONTEXT (103)",
    "DW_DLE_DIE_NO_CU_CONTEXT (104)",
    "DW_DLE_FIRST_DIE_NOT_CU (105)",
    "DW_DLE_NEXT_DIE_PTR_NULL (106)",
    "DW_DLE_DEBUG_FRAME_DUPLICATE  (107) Only one .debug_frame  "
        "section is allowed",
    "DW_DLE_DEBUG_FRAME_NULL (108) .debug_frame section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_ABBREV_DECODE_ERROR (109)",
    "DW_DLE_DWARF_ABBREV_NULL (110)",
    "DW_DLE_ATTR_NULL (111)",
    "DW_DLE_DIE_BAD (112)",
    "DW_DLE_DIE_ABBREV_BAD (113)",
    "DW_DLE_ATTR_FORM_BAD (114)",
    "DW_DLE_ATTR_NO_CU_CONTEXT (115)",
    "DW_DLE_ATTR_FORM_SIZE_BAD (116)",
    "DW_DLE_ATTR_DBG_NULL (117)",
    "DW_DLE_BAD_REF_FORM (118)",
    "DW_DLE_ATTR_FORM_OFFSET_BAD (119)",
    "DW_DLE_LINE_OFFSET_BAD (120)",
    "DW_DLE_DEBUG_STR_OFFSET_BAD (121)",
    "DW_DLE_STRING_PTR_NULL (122)",
    "DW_DLE_PUBNAMES_VERSION_ERROR (123)",
    "DW_DLE_PUBNAMES_LENGTH_BAD (124)",
    "DW_DLE_GLOBAL_NULL (125)",
    "DW_DLE_GLOBAL_CONTEXT_NULL (126)",
    "DW_DLE_DIR_INDEX_BAD (127)",
    "DW_DLE_LOC_EXPR_BAD (128)",
    "DW_DLE_DIE_LOC_EXPR_BAD (129)",
    "DW_DLE_ADDR_ALLOC (130)",
    "DW_DLE_OFFSET_BAD (131)",
    "DW_DLE_MAKE_CU_CONTEXT_FAIL (132)",
    "DW_DLE_REL_ALLOC (133)",
    "DW_DLE_ARANGE_OFFSET_BAD (134)",
    "DW_DLE_SEGMENT_SIZE_BAD (135) Size of a segment selector should usually be less than 8 (bytes).",
    "DW_DLE_ARANGE_LENGTH_BAD (136)",
    "DW_DLE_ARANGE_DECODE_ERROR (137)",
    "DW_DLE_ARANGES_NULL (138)",
    "DW_DLE_ARANGE_NULL (139)",
    "DW_DLE_NO_FILE_NAME (140)",
    "DW_DLE_NO_COMP_DIR (141)",
    "DW_DLE_CU_ADDRESS_SIZE_BAD (142)",
    "DW_DLE_INPUT_ATTR_BAD (143)",
    "DW_DLE_EXPR_NULL (144)",
    "DW_DLE_BAD_EXPR_OPCODE (145)",
    "DW_DLE_EXPR_LENGTH_BAD (146)",
    "DW_DLE_MULTIPLE_RELOC_IN_EXPR (147)",
    "DW_DLE_ELF_GETIDENT_ERROR (148)",
    "DW_DLE_NO_AT_MIPS_FDE (149)",
    "DW_DLE_NO_CIE_FOR_FDE (150)",
    "DW_DLE_DIE_ABBREV_LIST_NULL (151)",
    "DW_DLE_DEBUG_FUNCNAMES_DUPLICATE (152)",
    "DW_DLE_DEBUG_FUNCNAMES_NULL (153) .debug_funcnames section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_FUNCNAMES_VERSION_ERROR (154)",
    "DW_DLE_DEBUG_FUNCNAMES_LENGTH_BAD (155)",
    "DW_DLE_FUNC_NULL (156)",
    "DW_DLE_FUNC_CONTEXT_NULL (157)",
    "DW_DLE_DEBUG_TYPENAMES_DUPLICATE (158)",
    "DW_DLE_DEBUG_TYPENAMES_NULL (159) .debug_typenames section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_TYPENAMES_VERSION_ERROR (160)",
    "DW_DLE_DEBUG_TYPENAMES_LENGTH_BAD (161)",
    "DW_DLE_TYPE_NULL (162)",
    "DW_DLE_TYPE_CONTEXT_NULL (163)",
    "DW_DLE_DEBUG_VARNAMES_DUPLICATE (164)",
    "DW_DLE_DEBUG_VARNAMES_NULL (165) .debug_varnames section present but "
        "elf_getdata() failed or section is zero-length",
    "DW_DLE_DEBUG_VARNAMES_VERSION_ERROR (166)",
    "DW_DLE_DEBUG_VARNAMES_LENGTH_BAD (167)",
    "DW_DLE_VAR_NULL (168)",
    "DW_DLE_VAR_CONTEXT_NULL (169)",
    "DW_DLE_DEBUG_WEAKNAMES_DUPLICATE (170)",
    "DW_DLE_DEBUG_WEAKNAMES_NULL (171) .debug_weaknames section present but "
        "elf_getdata() failed or section is zero-length",

    "DW_DLE_DEBUG_WEAKNAMES_VERSION_ERROR (172)",
    "DW_DLE_DEBUG_WEAKNAMES_LENGTH_BAD (173)",
    "DW_DLE_WEAK_NULL (174)",
    "DW_DLE_WEAK_CONTEXT_NULL (175)",
    "DW_DLE_LOCDESC_COUNT_WRONG (176)",
    "DW_DLE_MACINFO_STRING_NULL (177)",
    "DW_DLE_MACINFO_STRING_EMPTY (178)",
    "DW_DLE_MACINFO_INTERNAL_ERROR_SPACE (179)",
    "DW_DLE_MACINFO_MALLOC_FAIL (180)",
    "DW_DLE_DEBUGMACINFO_ERROR (181)",
    "DW_DLE_DEBUG_MACRO_LENGTH_BAD (182)",
    "DW_DLE_DEBUG_MACRO_MAX_BAD (183)",
    "DW_DLE_DEBUG_MACRO_INTERNAL_ERR (184)",
    "DW_DLE_DEBUG_MACRO_MALLOC_SPACE (185)",
    "DW_DLE_DEBUG_MACRO_INCONSISTENT (186)",
    "DW_DLE_DF_NO_CIE_AUGMENTATION(187)",
    "DW_DLE_DF_REG_NUM_TOO_HIGH(188)",
    "DW_DLE_DF_MAKE_INSTR_NO_INIT(189)",
    "DW_DLE_DF_NEW_LOC_LESS_OLD_LOC(190)",
    "DW_DLE_DF_POP_EMPTY_STACK(191)",
    "DW_DLE_DF_ALLOC_FAIL(192)",
    "DW_DLE_DF_FRAME_DECODING_ERROR(193)",
    "DW_DLE_DEBUG_LOC_SECTION_SHORT(194)",
    "DW_DLE_FRAME_AUGMENTATION_UNKNOWN(195)",
    "DW_DLE_PUBTYPE_CONTEXT(196)",
    "DW_DLE_DEBUG_PUBTYPES_LENGTH_BAD(197)",
    "DW_DLE_DEBUG_PUBTYPES_VERSION_ERROR(198)",
    "DW_DLE_DEBUG_PUBTYPES_DUPLICATE(199)",
    "DW_DLE_FRAME_CIE_DECODE_ERROR(200)",
    "DW_DLE_FRAME_REGISTER_UNREPRESENTABLE(201)",
    "DW_DLE_FRAME_REGISTER_COUNT_MISMATCH(202)",
    "DW_DLE_LINK_LOOP(203)",
    "DW_DLE_STRP_OFFSET_BAD(204)",
    "DW_DLE_DEBUG_RANGES_DUPLICATE(205)",
    "DW_DLE_DEBUG_RANGES_OFFSET_BAD(206)",
    "DW_DLE_DEBUG_RANGES_MISSING_END(207)",
    "DW_DLE_DEBUG_RANGES_OUT_OF_MEM(208)",
    "DW_DLE_DEBUG_SYMTAB_ERR(209)",
    "DW_DLE_DEBUG_STRTAB_ERR(210)",
    "DW_DLE_RELOC_MISMATCH_INDEX(211)",
    "DW_DLE_RELOC_MISMATCH_RELOC_INDEX(212)",
    "DW_DLE_RELOC_MISMATCH_STRTAB_INDEX(213)",
    "DW_DLE_RELOC_SECTION_MISMATCH(214)",
    "DW_DLE_RELOC_SECTION_MISSING_INDEX(215)",
    "DW_DLE_RELOC_SECTION_LENGTH_ODD(216)",
    "DW_DLE_RELOC_SECTION_PTR_NULL(217)",
    "DW_DLE_RELOC_SECTION_MALLOC_FAIL(218)",
    "DW_DLE_NO_ELF64_SUPPORT(219)",
    "DW_DLE_MISSING_ELF64_SUPPORT(220)",
    "DW_DLE_ORPHAN_FDE(221)",
    "DW_DLE_DUPLICATE_INST_BLOCK(222)",
    "DW_DLE_BAD_REF_SIG8_FORM(223)",
    "DW_DLE_ATTR_EXPRLOC_FORM_BAD(224)",
    "DW_DLE_FORM_SEC_OFFSET_LENGTH_BAD(225)",
    "DW_DLE_NOT_REF_FORM(226)",
    "DW_DLE_DEBUG_FRAME_LENGTH_NOT_MULTIPLE(227)",
    "DW_DLE_REF_SIG8_NOT_HANDLED (228)",
    "DW_DLE_DEBUG_FRAME_POSSIBLE_ADDRESS_BOTCH (229)",
    "DW_DLE_LOC_BAD_TERMINATION (230) the last location operator in an expression is missing some associated data, an operator ended too soon",
    "DW_DLE_SYMTAB_SECTION_LENGTH_ODD (231) so doing relocations seems unsafe",
    "DW_DLE_RELOC_SECTION_SYMBOL_INDEX_BAD (232) so doing a relocation seems unsafe",
    "DW_DLE_RELOC_SECTION_RELOC_TARGET_SIZE_UNKNOWN (233) so doing a relocation is unsafe",
    "DW_DLE_SYMTAB_SECTION_ENTRYSIZE_ZERO(234)",
    "DW_DLE_LINE_NUMBER_HEADER_ERROR (235), a line number program header seems incomplete (perhaps the header_length is wrong?).",
    "DW_DLE_DEBUG_TYPES_NULL (236)",
    "DW_DLE_DEBUG_TYPES_DUPLICATE (237)",
    "DW_DLE_DEBUG_TYPES_ONLY_DWARF4 (238) DW4 and DW5 have types CUs",
    "DW_DLE_DEBUG_TYPEOFFSET_BAD (239)",
    "DW_DLE_GNU_OPCODE_ERROR (240)",
    "DW_DLE_DEBUGPUBTYPES_ERROR (241), could not create pubtypes section",
    "DW_DLE_AT_FIXUP_NULL (242)",
    "DW_DLE_AT_FIXUP_DUP (243)",
    "DW_DLE_BAD_ABINAME (244)",
    "DW_DLE_TOO_MANY_DEBUG(245), too many .debug_* sections present somehow",
    "DW_DLE_DEBUG_STR_OFFSETS_DUPLICATE(246)",
    "DW_DLE_SECTION_DUPLICATION(247)",
    "DW_DLE_SECTION_ERROR(248)",
    "DW_DLE_DEBUG_ADDR_DUPLICATE(249)",
    "DW_DLE_DEBUG_CU_UNAVAILABLE_FOR_FORM(250)",
    "DW_DLE_DEBUG_FORM_HANDLING_INCOMPLETE(251)",
    "DW_DLE_NEXT_DIE_PAST_END(252)",
    "DW_DLE_NEXT_DIE_WRONG_FORM(253)",
    "DW_DLE_NEXT_DIE_NO_ABBREV_LIST(254)",
    "DW_DLE_NESTED_FORM_INDIRECT_ERROR(255)",
    "DW_DLE_CU_DIE_NO_ABBREV_LIST(256)",
    "DW_DLE_MISSING_NEEDED_DEBUG_ADDR_SECTION(257)",
    "DW_DLE_ATTR_FORM_NOT_ADDR_INDEX(258)",
    "DW_DLE_ATTR_FORM_NOT_STR_INDEX(259)",
    "DW_DLE_DUPLICATE_GDB_INDEX(260)",
    "DW_DLE_ERRONEOUS_GDB_INDEX_SECTION(261) The section is too small",
    "DW_DLE_GDB_INDEX_COUNT_ERROR(262)",
    "DW_DLE_GDB_INDEX_COUNT_ADDR_ERROR(263)",
    "DW_DLE_GDB_INDEX_CUVEC_ERROR(264)",
    "DW_DLE_GDB_INDEX_INDEX_ERROR(265)",
    "DW_DLE_DUPLICATE_CU_INDEX(266)",
    "DW_DLE_DUPLICATE_TU_INDEX(267)",
    "DW_DLE_XU_TYPE_ARG_ERROR(268) XU means dwarf_cu_ or tu_ index section",
    "DW_DLE_XU_IMPOSSIBLE_ERROR(269) XU means dwarf_cu_ or tu_ index section",
    "DW_DLE_XU_NAME_COL_ERROR(270) XU means dwarf_cu_ or tu_ index section",
    "DW_DLE_XU_HASH_ROW_ERROR(271) XU means dwarf_cu_ or tu_ index section",
    "DW_DLE_XU_HASH_INDEX_ERROR(272) XU means dwarf_cu_ or tu_ index section",
    "DW_DLE_FAILSAFE_ERRVAL(273)",
    "DW_DLE_ARANGE_ERROR(274) producer problem in object generation",
    "DW_DLE_PUBNAMES_ERROR(275) producer problem in object generation",
    "DW_DLE_FUNCNAMES_ERROR(276) producer problem in object generation",
    "DW_DLE_TYPENAMES_ERROR(277) producer problem in object generation",
    "DW_DLE_VARNAMES_ERROR(278) producer problem in object generation",
    "DW_DLE_WEAKNAMES_ERROR(279) producer problem in object generation",
    "DW_DLE_RELOCS_ERROR(280) producer problem in object generation",
    "DW_DLE_DW_DLE_ATTR_OUTSIDE_SECTION(281)",
    "DW_DLE_FISSION_INDEX_WRONG(282)",
    "DW_DLE_FISSION_VERSION_ERROR(283)",
    "DW_DLE_NEXT_DIE_LOW_ERROR(284) corrupted DIE tree",
    "DW_DLE_CU_UT_TYPE_ERROR(285) not a valid DW_UT_* value",
    "DW_DLE_NO_SUCH_SIGNATURE_FOUND(286) CU signature not in the index",
    "DW_DLE_SIGNATURE_SECTION_NUMBER_WRONG(287) libdwarf software error",
    "DW_DLE_ATTR_FORM_NOT_DATA8(288) wanted an 8 byte signature",
    "DW_DLE_SIG_TYPE_WRONG_STRING (289) expected tu or cu",
    "DW_DLE_MISSING_REQUIRED_TU_OFFSET_HASH(290) is a broken dwp package file",
    "DW_DLE_MISSING_REQUIRED_CU_OFFSET_HASH(291) is a broken dwp package file",
    "DW_DLE_DWP_MISSING_DWO_ID(292)",
    "DW_DLE_DWP_SIBLING_ERROR(293)",
    "DW_DLE_DEBUG_FISSION_INCOMPLETE(294)",
    "DW_DLE_FISSION_SECNUM_ERR(295) internal libdwarf error",
    "DW_DLE_DEBUG_MACRO_DUPLICATE(296)",
    "DW_DLE_DEBUG_NAMES_DUPLICATE(297)",
    "DW_DLE_DEBUG_LINE_STR_DUPLICATE(298)",
    "DW_DLE_DEBUG_SUP_DUPLICATE(299)",
    "DW_DLE_NO_SIGNATURE_TO_LOOKUP(300)",
    "DW_DLE_NO_TIED_ADDR_AVAILABLE(301)",
    "DW_DLE_NO_TIED_SIG_AVAILABLE(302)",
    "DW_DLE_STRING_NOT_TERMINATED(303) section data may be corrupted",
    "DW_DLE_BAD_LINE_TABLE_OPERATION(304) two-level line table botch",
    "DW_DLE_LINE_CONTEXT_BOTCH(305) call is wrong or memory corruption",
    "DW_DLE_LINE_CONTEXT_INDEX_WRONG(306)",
    "DW_DLE_NO_TIED_STRING_AVAILABLE(307) tied file does not have the string",
    "DW_DLE_NO_TIED_FILE_AVAILABLE(308) see dwarf_set_tied_dbg()",
    "DW_DLE_CU_TYPE_MISSING(309) libdwarf bug or data corruption",
    "DW_DLE_LLE_CODE_UNKNOWN (310) libdwarf bug or data corruption",
    "DW_DLE_LOCLIST_INTERFACE_ERROR (311) interface cannot do location or DW_OP*",
    "DW_DLE_LOCLIST_INDEX_ERROR (312)",
    "DW_DLE_INTERFACE_NOT_SUPPORTED (313)",
    "DW_DLE_ZDEBUG_REQUIRES_ZLIB (314) Unable to decompress .zdebug as zlib missing",
    "DW_DLE_ZDEBUG_INPUT_FORMAT_ODD(315)",
    "DW_DLE_ZLIB_BUF_ERROR (316) Z_BUF_ERROR buffer size small",
    "DW_DLE_ZLIB_DATA_ERROR (317) Z_DATA_ERROR compressed data corrupted",
    "DW_DLE_MACRO_OFFSET_BAD (318)",
    "DW_DLE_MACRO_OPCODE_BAD (319)",
    "DW_DLE_MACRO_OPCODE_FORM_BAD (320)",
    "DW_DLE_UNKNOWN_FORM (321) Possibly corrupt DWARF data",
    "DW_DLE_BAD_MACRO_HEADER_POINTER(322)",
    "DW_DLE_BAD_MACRO_INDEX(323)",
    "DW_DLE_MACRO_OP_UNHANDLED(324) Possibly an implementation extension",
    "DW_DLE_MACRO_PAST_END(325)",
    "DW_DLE_LINE_STRP_OFFSET_BAD(326)",
    "DW_DLE_STRING_FORM_IMPROPER(327) An internal libdwarf logic error",
    "DW_DLE_ELF_FLAGS_NOT_AVAILABLE(328) elf/non-elf object confusion?",
    "DW_DLE_LEB_IMPROPER (329) Runs off end of section or CU",
    "DW_DLE_DEBUG_LINE_RANGE_ZERO (330) Corrupted line section",
    "DW_DLE_READ_LITTLEENDIAN_ERROR (331) Corrupted dwarfdata littleendian host",
    "DW_DLE_READ_BIGENDIAN_ERROR (332) Corrupted dwarf data bigendian host",
    "DW_DLE_RELOC_INVALID (333) relocation corruption",
    "DW_DLE_INFO_HEADER_ERROR(334) Corrupt dwarf",
    "DW_DLE_ARANGES_HEADER_ERROR(335) Corrupt dwarf",
    "DW_DLE_LINE_OFFSET_WRONG_FORM(336) Corrupt dwarf",
    "DW_DLE_FORM_BLOCK_LENGTH_ERROR(337) Corrupt dwarf",
    "DW_DLE_ZLIB_SECTION_SHORT (338) Corrupt dwarf",
    "DW_DLE_CIE_INSTR_PTR_ERROR (339)",
    "DW_DLE_FDE_INSTR_PTR_ERROR (340)",
    "DW_DLE_FISSION_ADDITION_ERROR (341) Corrupt dwarf",
    "DW_DLE_HEADER_LEN_BIGGER_THAN_SECSIZE (342) Corrupt dwarf",
    "DW_DLE_LOCEXPR_OFF_SECTION_END (343) Corrupt dwarf",
    "DW_DLE_POINTER_SECTION_UNKNOWN (344)",
    "DW_DLE_ERRONEOUS_XU_INDEX_SECTION(345) XU means cu_ or tu_ index",
};

#ifdef TESTING
#define FALSE 0
#define TRUE 1
/* This is just to help localize the error. */
static void
printone(int i)
{
    unsigned arraysize = sizeof(_dwarf_errmsgs) / sizeof(char *);
    if ( i >= arraysize) {
        printf("%d is outside the array! Missing something!\n",i);
    } else {
        printf("%d is: %s\n",i,_dwarf_errmsgs[i]);
    }
}

/* Arbitrary. A much smaller max length value would work. */
#define MAX_NUM_LENGTH 12

/* return TRUE on error */
static int
check_errnum_mismatches(unsigned i)
{
    unsigned nextstop = 0;
    const char *sp = _dwarf_errmsgs[i];
    const char *cp = sp;
    unsigned innit = FALSE;
    unsigned prevchar = 0;
    unsigned value = 0;

    for( ; *cp; cp++) {
        unsigned c = 0;
        c = 0xff & *cp;
        if ( c >= '0' && c <= '9' && !innit
            && prevchar != '(') {
            /* Skip. number part of macro name. */
            prevchar = c;
            continue;
        }
        if ( c >= '0' && c <= '9') {
            value = value * 10;
            value += (c - '0');
            nextstop++;
            if (nextstop > MAX_NUM_LENGTH) {
                break;
            }
            innit = TRUE;
        } else {
            if (innit) {
                break;
            }
            prevchar= c;
        }
    }
    if (innit) {
        if (i != value) {
            return TRUE;
        }
        return FALSE;
    }
    /* There is no number to check. Ignore it. */
    printf("mismatch value %d has no errnum to check %s\n",i,_dwarf_errmsgs[i]);
    return TRUE;
}

int
main()
{
    unsigned arraysize = sizeof(_dwarf_errmsgs) / sizeof(char *);
    unsigned i = 0;

    if (arraysize != (DW_DLE_LAST + 1)) {
        printf("Missing or extra entry in dwarf error strings array"
            " %u expected DW_DLE_LAST+1 %d\n",arraysize, DW_DLE_LAST+1);
        printone(1);
        printone(100);
        printone(200);
        printone(250);
        printone(260);
        printone(262);
        printone(263);
        printone(264);
        printone(265);
        printone(273);
        printone(274);
        printone(275);
        printone(300);
        printone(328);
        exit(1);
    }
    for ( i = 0; i <= DW_DLE_LAST; ++i) {
        if(check_errnum_mismatches(i)) {
            printf("mismatch value %d is: %s\n",i,_dwarf_errmsgs[i]);
            exit(1);
        }
    }
    /* OK. */
    exit(0);
}
#endif /* TESTING */
