/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2025 Ventana Micro Systems Inc.
 */

#ifndef __RPMI_MM_H__
#define __RPMI_MM_H__

#define MAX_BIT  (0x8000000000000000ULL)

#define ENCODE_ERROR(code)     ((rpmi_uint64_t)(MAX_BIT | code))
#define RETURN_ERROR(code)     (((rpmi_int64_t)(rpmi_uint64_t)(code)) < 0)

#define EFI_SUCCESS            ((rpmi_uint64_t)0)
#define EFI_INVALID_PARAMETER  ENCODE_ERROR(2)
#define EFI_UNSUPPORTED        ENCODE_ERROR(3)
#define EFI_BUFFER_TOO_SMALL   ENCODE_ERROR(5)
#define EFI_NOT_FOUND          ENCODE_ERROR(14)
#define EFI_ACCESS_DENIED      ENCODE_ERROR(15)

#define EFI_ERROR(n)           RETURN_ERROR(n)

/** Basic data type definitions introduced in UEFI */
struct efi_guid {
	rpmi_uint32_t data1;
	rpmi_uint16_t data2;
	rpmi_uint16_t data3;
	rpmi_uint8_t data4[8];
};

#define GUID_LENGTH  16

enum efi_mm_header_guid {
	EFI_MM_HDR_GUID_UNSUPPORTED,
	EFI_MM_HDR_GUID_NONE = EFI_MM_HDR_GUID_UNSUPPORTED,
	EFI_MM_VAR_PROTOCOL_GUID,
	EFI_MM_VAR_POLICY_GUID,
	EFI_MM_END_OF_DXE_GUID,
	EFI_MM_READY_TO_BOOT_GUID,
	EFI_MM_EXIT_BOOT_SVC_GUID,
};

#define EFI_MM_HDR_GUID_NONE_DATA	\
	{ 0x00000000, 0x0000, 0x0000,	\
	  { 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00 } }

#define EFI_MM_VAR_PROTOCOL_GUID_DATA	\
	{ 0xed32d533, 0x99e6, 0x4209,	\
	  { 0x9c, 0xc0, 0x2d, 0x72, 0xcd, 0xd9, 0x98, 0xa7 } }

#define EFI_MM_VAR_POLICY_GUID_DATA	\
	{ 0xda1b0d11, 0xd1a7, 0x46c4,	\
	  { 0x9d, 0xc9, 0xf3, 0x71, 0x48, 0x75, 0xc6, 0xeb } }

#define EFI_MM_END_OF_DXE_GUID_DATA	\
	{ 0x2ce967a, 0xdd7e, 0x4ffc, 	\
	  { 0x9e, 0xe7, 0x81, 0x0c, 0xf0, 0x47, 0x08, 0x80 } }

#define EFI_MM_READY_TO_BOOT_GUID_DATA	\
	{ 0x7ce88fb3, 0x4bd7, 0x4679,	\
	  { 0x87, 0xa8, 0xa8, 0xd8, 0xde, 0xe5, 0x0d, 0x2b } }

#define EFI_MM_EXIT_BOOT_SVC_GUID_DATA	\
	{ 0x27abf055, 0xb1b8, 0x4c26,	\
	  { 0x80, 0x48, 0x74, 0x8f, 0x37, 0xba, 0xa2, 0xdf } }

#pragma pack(1)

/**
 * struct efi_mm_comm_header - Header used for MM communication
 *
 * @hdr_guid:  guid used for disambiguation of message format
 * @msg_len:   size of data (in bytes) - excluding size of the header
 * @data:      an array of bytes that is msg_len in size
 *
 * To avoid confusion in interpreting frames, the communication buffer should
 * always begin with struct efi_mm_comm_header.
 */
struct efi_mm_comm_header {
	struct efi_guid hdr_guid;
	rpmi_uint64_t msg_len;
	rpmi_uint8_t data[1];
};

struct efi_var_policy_comm_header {
	rpmi_uint32_t signature;
	rpmi_uint32_t revision;
	rpmi_uint32_t command;
	rpmi_uint64_t result;
};

#pragma pack()

/** The payload for this function is struct mm_var_comm_access_variable */
#define MM_VAR_FN_GET_VARIABLE                         1

/** The payload for this function is struct mm_var_comm_get_next_var_name */
#define MM_VAR_FN_GET_NEXT_VARIABLE_NAME               2

/** The payload for this function is struct mm_var_comm_access_variable */
#define MM_VAR_FN_SET_VARIABLE                         3

#define MM_VAR_FN_QUERY_VARIABLE_INFO                  4
#define MM_VAR_FN_READY_TO_BOOT                        5
#define MM_VAR_FN_EXIT_BOOT_SERVICE                    6
#define MM_VAR_FN_GET_STATISTICS                       7
#define MM_VAR_FN_LOCK_VARIABLE                        8
#define MM_VAR_FN_VAR_CHECK_VARIABLE_PROPERTY_SET      9
#define MM_VAR_FN_VAR_CHECK_VARIABLE_PROPERTY_GET      10

/** The payload for this function is struct mm_var_comm_get_payload_size */
#define MM_VAR_FN_GET_PAYLOAD_SIZE                     11

#define MM_VAR_FN_INIT_RUNTIME_VARIABLE_CACHE_CONTEXT  12
#define MM_VAR_FN_SYNC_RUNTIME_CACHE                   13
#define MM_VAR_FN_GET_RUNTIME_CACHE_INFO               14

/** Size of MM communicate header, without including the payload */
#define MM_COMM_HEADER_SIZE  (offsetof(struct efi_mm_comm_header, data))

/** Size of MM variable communicate header, without including the payload */
#define MM_VAR_COMM_HEADER_SIZE  (offsetof(struct mm_var_comm_header, data))

/* Max information size per MM variable: 1 KB (including header) */
#define MAX_VARINFO_SIZE  1024
#define MAX_PAYLOAD_SIZE  (MAX_VARINFO_SIZE - MM_VAR_COMM_HEADER_SIZE)

/**
 * This structure is used for MM variable. The communication buffer should be:
 *      struct efi_mm_comm_header + struct mm_var_comm_header + payload
 */
struct mm_var_comm_header {
	rpmi_uint64_t function;
	rpmi_uint64_t return_status;
	rpmi_uint8_t data[1];
};

/** This structure is used to communicate with MM via SetVariable/GetVariable */
struct mm_var_comm_access_variable {
	struct efi_guid guid;
	rpmi_uint64_t datasize;
	rpmi_uint64_t namesize;
	rpmi_uint32_t attr;
	rpmi_uint16_t name[1];
};

/** This structure is used to communicate with MM via GetNextVariableName */
struct mm_var_comm_get_next_var_name {
	struct efi_guid guid;
	rpmi_uint64_t namesize;	// Return name buffer size
	rpmi_uint16_t name[1];
};

struct mm_var_comm_get_payload_size {
	rpmi_uint64_t var_payload_size;
};

#endif /* __RPMI_MM_H__ */
