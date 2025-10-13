/* SPDX-License-Identifier: BSD-2-Clause */
/*
 * Copyright (c) 2025 Ventana Micro Systems Inc.
 */

#ifndef __RPMI_MM_COMMON_H__
#define __RPMI_MM_COMMON_H__

#ifdef DEBUG
#define PREFIX_STR  "== LIBRPMI: MM: =========> %19s: %03u: "

#define DPRINTF(msg...)							\
	{								\
		rpmi_env_printf(PREFIX_STR, __func__, __LINE__);	\
		rpmi_env_printf(msg);					\
		rpmi_env_printf("\n");					\
	}
#else
#define DPRINTF(msg...)
#endif

struct rpmi_service_group_mm {
	rpmi_uint32_t mm_version;
	struct rpmi_mm mmi;
	struct rpmi_service_group group;
};

struct rpmi_mm_comm_req {
	rpmi_uint32_t idata_off;
	rpmi_uint32_t idata_len;
	rpmi_uint32_t odata_off;
	rpmi_uint32_t odata_len;
};

enum rpmi_error mm_efi_communicate(struct rpmi_service_group *group,
				   struct rpmi_service *service,
				   struct rpmi_transport *xport,
				   rpmi_uint16_t request_datalen,
				   const rpmi_uint8_t *request_data,
				   rpmi_uint16_t *response_datalen,
				   rpmi_uint8_t *response_data);

#ifdef RPMI_MM_COMMON_OWNER

typedef enum rpmi_error (*comm_handlers)(struct rpmi_service_group *group,
					 struct rpmi_service *service,
					 struct rpmi_transport *xport,
					 rpmi_uint16_t request_datalen,
					 const rpmi_uint8_t *request_data,
					 rpmi_uint16_t *response_datalen,
					 rpmi_uint8_t *response_data);

static comm_handlers mm_service_handlers[RPMI_MM_SERVICE_MAX] = {
	[RPMI_MM_SERVICE_EFI] = mm_efi_communicate,
};

#endif /* RPMI_MM_COMMON_OWNER */

#endif /* __RPMI_MM_COMMON_H__ */
