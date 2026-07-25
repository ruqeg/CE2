#pragma once

#include <engine/core/alias.h>

typedef struct crude_gfx_rhi crude_gfx_rhi;

CRUDE_API void
crude_gfx_rhi_initialize_null
(
  _In_ crude_gfx_rhi                                      *rhi
);

CRUDE_API void
crude_gfx_rhi_deinitialize_null
(
  _In_ crude_gfx_rhi                                      *rhi
);