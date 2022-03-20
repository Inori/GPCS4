#pragma once

#include "GPCS4Common.h"
#include "GnmGpuAddressCommon.h"
#include "GnmErrorGen.h"

#include "Gnm/GnmConstant.h"
#include "Gnm/GnmDataFormat.h"
#include "Gnm/GnmStructure.h"

#include <cstring>


namespace sce
{
	namespace Gnm
	{
		class DepthRenderTarget;  // gnm/depthtarget.h
		class Texture;            // gnm/texture.h
		class RenderTarget;       // gnm/rendertarget.h
		struct SizeAlign;         // gnm/gpumem.h
	}

	/** @brief The namespace for the GpuAddress library.
	*/
	namespace GpuAddress
	{

		/** @brief Defines the error codes returned by GpuAddress library functions.
		*/
		typedef enum Status
		{
			kStatusSuccess             = 0,												///< The operation was successful.
			kStatusInvalidArgument     = SCE_GPU_ADDRESS_ERROR_STATUS_INVALID_ARGUMENT,	///< The operation failed due to an invalid argument.
			kStatusInternalTilingError = SCE_GPU_ADDRESS_ERROR_STATUS_INTERNAL_TILING,	///< The operation failed due to an internal tiling error.

		} Status;

		/** @brief List of common surface types.
		 *
		 * Used to generate the correct value for SurfaceFlags using getFlagsForSurfaceType().
		 */
		typedef enum SurfaceType
		{
			kSurfaceTypeColorTargetDisplayable = 1,  ///< A render target that will be sent to scan-out. This may have different restrictions than a general-purpose color buffer.
			kSurfaceTypeColorTarget            = 2,  ///< An intermediate render target.
            kSurfaceTypeDepthTarget            = 3,  ///< A depth target that supports a depth buffer and/or a stencil buffer.
			kSurfaceTypeDepthOnlyTarget        = 4,  ///< A depth target that supports a depth buffer, but may not also support a stencil buffer.
			kSurfaceTypeStencilOnlyTarget      = 5,  ///< A depth target that supports a stencil buffer, but may not also support a depth buffer.
			kSurfaceTypeFmaskBuffer            = 6,  ///< An FMASK surface.
			kSurfaceTypeTextureFlat            = 7,  ///< A read-only 1D or 2D texture (or texture array).
			kSurfaceTypeTextureVolume          = 8,  ///< A read-only 3D texture.
			kSurfaceTypeTextureCubemap         = 9,  ///< A read-only cubic environment map (or cubemap array).
			kSurfaceTypeRwTextureFlat          = 10, ///< A read/write 1D or 2D texture (or texture array).
			kSurfaceTypeRwTextureVolume        = 11, ///< A read/write 3D texture.
			kSurfaceTypeRwTextureCubemap       = 12, ///< A read/write cubic environment map (or cubemap array).
		} SurfaceType;

		/** @brief List of mipmap modes.
		 *	
		 *	Used to indicate whether a surface has mipmaps.
		 *
		 *	@see getFlagsForSurfaceType()
		 */
		typedef enum SurfaceMipmapMode
		{
			kSurfaceMipmapDisable = 0,	///< Surface has no mipmaps beyond the base level.
			kSurfaceMipmapEnable  = 1,	///< Surface has one or more mipmaps beyond the base level.
		} SurfaceMipmapMode;


		////////////////////////////////////////////////////////
		// Surface Tiling Functions
		////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201) // nameless struct/union
#endif
		/**
		 * @brief Flags describing the intended use of a surface.
		 *
		 * Used to guide low-level tiling code.
		 *
		 * @sa getFlagsForSurfaceType()
		 */
		typedef union SurfaceFlags
		{
			/** @brief Exposes the individual surface flags as bits. */
			class
			{
			public:
				uint32_t m_colorTarget   : 1; ///< DEPRECATED -- Unused.
				uint32_t m_depthTarget   : 1; ///< Flag indicates whether the surface is a depth-only buffer.
				uint32_t m_stencilTarget : 1; ///< Flag indicates whether the surface is a stencil-only buffer.
				uint32_t m_texture       : 1; ///< DEPRECATED -- Unused.
				uint32_t m_cube          : 1; ///< Flag indicates whether the surface is a cubemap.
				uint32_t m_volume        : 1; ///< Flag indicates whether the surface is a volume texture.
				uint32_t m_fmask         : 1; ///< Flag indicates whether the surface is an FMASK surface.
				uint32_t m_cubeAsArray   : 1; ///< DEPRECATED -- Unused.
				uint32_t m_overlay       : 1; ///< DEPRECATED -- Unused.
				uint32_t m_noStencil     : 1; ///< DEPRECATED -- Unused.
				uint32_t m_display       : 1; ///< DEPRECATED -- Unused.
				uint32_t m_prt           : 1; ///< DEPRECATED -- Unused.
				uint32_t m_pow2Pad       : 1; ///< If set, all dimensions will be padded to powers of 2. <i>Must</i> be set for any texture with mipmaps, including the base level.
				uint32_t m_texCompatible : 1; ///< Flag indicates whether the surface must be configured such that it can be used/aliased as a Texture. NEO ONLY.
				uint32_t m_reserved      :18; ///< Reserved bits.
			};

			uint32_t m_value; ///< All flags as an integer.
		} SurfaceFlags;
#ifdef _MSC_VER
#pragma warning(pop)
#endif

		/**
		 * @brief Describes a rectangular region of a render surface.
		 */
		typedef struct SurfaceRegion
		{
			uint32_t m_left;   ///< The x position of the left side of the region. For surface formats with multiple texels per element (such as BCn or R1), this member must be specified in units of elements, not individual texels.
			uint32_t m_top;    ///< The y position of the top of the region. For surface formats with multiple texels per element (such as BCn or R1), this member must be specified in units of elements, not individual texels.
			uint32_t m_front;  ///< The z position of the front of the region. For non-volume surfaces, specify a value of <c>0</c>.
			uint32_t m_right;  ///< The x position of the right side of the region. For surface formats with multiple texels per element (such as BCn or R1), this member must be specified in units of elements, not individual texels. The element at <i>x=m_right</i> is NOT included in the region. 
			uint32_t m_bottom; ///< The y position of the bottom of the region. For surface formats with multiple texels per element (such as BCn or R1), this member must be specified in units of elements, not individual texels. The element at <i>y=m_bottom</i> is NOT included in the region.
			uint32_t m_back;   ///< The z position of the back of the region. For non-volume surfaces, a value of <c>1</c> should be specified. The element at <i>z=m_back</i> is NOT included in the region.
		} SurfaceRegion; 

		/**
		 * @brief Gets the correct surface flags for a specified surface type.
		 *
		 * It is permissible to bypass this function and create the flags manually, but this is not recommended; invalid
		 * combinations of flags may lead to unexpected and undefined behavior.
		 *
		 * @param[in] minGpuMode    The minimum GPU mode with which this surface must be compatible. This value influences various tiling computations.
		 * @param[out] outFlags		Receives the surface flags, which will be expected by other surface-tiling functions of the specified surface type.
		 *							This pointer must not be <c>NULL</c>.
		 * @param[in] surfaceType	The type of surface for which this function generates flags.
		 * @param[in] mipmapMode    Specifies whether the SurfaceFlags::m_pow2pad flag is set. If the surface is a Texture with mipmaps, pass <c>kSurfaceMipmapEnable</c>.
		 *                          For base-level-only Textures, pass <c>kSurfaceMipmapDisable</c>. For non-Texture surface types, this parameter will be ignored.
		 *
		 * @return                  A status code from GpuAddress::Status.
		 *
		 * @sa SurfaceFlags
		 */
		int32_t getFlagsForSurfaceType(Gnm::GpuMode minGpuMode, SurfaceFlags *outFlags, SurfaceType surfaceType, SurfaceMipmapMode mipmapMode);

		/** 
		 * @deprecated This function now takes a Gnm::GpuMode argument.
		 * @brief Gets the correct surface flags for a specified surface type.
		 *
		 * It is permissible to bypass this function and create the flags manually, but this is not recommended; invalid
		 * combinations of flags may lead to unexpected and undefined behavior.
		 *
		 * @param[out] outFlags		Receives the surface flags, which will be expected by other surface-tiling functions of the specified surface type.
		 *							This pointer must not be <c>NULL</c>.
		 * @param[in] surfaceType	The type of surface for which this function generates flags.
		 * @param[in] mipmapMode    Specifies whether the SurfaceFlags::m_pow2pad flag is set. If the surface is a Texture with mipmaps, pass <c>kSurfaceMipmapEnable</c>.
		 *                          For base-level-only Textures, pass <c>kSurfaceMipmapDisable</c>. For non-Texture surface types, this parameter will be ignored.
		 *
		 * @return                  A status code from GpuAddress::Status.
		 *
		 * @sa SurfaceFlags
		 */
		
		inline int32_t getFlagsForSurfaceType(SurfaceFlags* outFlags, SurfaceType surfaceType, SurfaceMipmapMode mipmapMode)
		{
			return getFlagsForSurfaceType(Gnm::kGpuModeBase, outFlags, surfaceType, mipmapMode);
		}

		/**
		 * @brief Gets the appropriate array mode for a specified surface type.
		 *
		 * @param[in] minGpuMode            The minimum GPU mode with which this surface must be compatible. This value influences various tiling computations.
		 * @param[out] outArrayMode			Receives the appropriate array mode for the specified surface type.
		 *									This pointer must not be <c>NULL</c>.
		 * @param[in] surfaceType			The type of surface for which this function gets the appropriate array mode.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel, for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *									Values greater than 1 can influence the appropriate array mode.
		 *
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa Gnm::ArrayMode
		 */
		int32_t getArrayModeForSurfaceType(Gnm::GpuMode minGpuMode, Gnm::ArrayMode *outArrayMode, SurfaceType surfaceType, uint32_t numFragmentsPerPixel);

		/**
		 * @deprecated This function now takes a Gnm::GpuMode argument.
		 * @brief Gets the appropriate array mode for a specified surface type.
		 *
		 * @param[out] outArrayMode			Receives the appropriate array mode for the specified surface type.
		 *									This pointer must not be <c>NULL</c>.
		 * @param[in] surfaceType			The type of surface for which this function gets the appropriate array mode.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel, for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *									Values greater than 1 can influence the appropriate array mode.
		 *
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa Gnm::ArrayMode
		 */
		
		inline int32_t getArrayModeForSurfaceType(Gnm::ArrayMode* outArrayMode, SurfaceType surfaceType, uint32_t numFragmentsPerPixel)
		{
			return getArrayModeForSurfaceType(Gnm::kGpuModeBase, outArrayMode, surfaceType, numFragmentsPerPixel);
		}

		/**
		 * @brief Gets the appropriate micro-tile mode for a specified surface type.
		 *
		 * @param[in] minGpuMode        The minimum GPU mode with which this surface must be compatible. This value influences various tiling computations.
		 * @param[out] outMicroTileMode	Receives the appropriate micro-tile mode for the specified surface type.
		 *							    This pointer must not be <c>NULL</c>.
		 * @param[in] surfaceType		The type of surface for which this function gets the appropriate micro-tile mode.
		 *
		 * @return                      A status code from GpuAddress::Status.
		 *
		 * @sa Gnm::MicroTileMode
		 */
		int32_t getMicroTileModeForSurfaceType(Gnm::GpuMode minGpuMode, Gnm::MicroTileMode *outMicroTileMode, SurfaceType surfaceType);

		/**
		 * @deprecated This function now takes a Gnm::GpuMode argument.
		 * @brief Gets the appropriate micro-tile mode for a specified surface type.
		 *
		 * @param[out] outMicroTileMode	Receives the appropriate micro-tile mode for the specified surface type.
		 *							    This pointer must not be <c>NULL</c>.
		 * @param[in] surfaceType		Values greater than 1 can influence the appropriate array mode.
		 *
		 * @return                      A status code from GpuAddress::Status.
		 *
		 * @sa Gnm::MicroTileMode
		 */
		
		int32_t getMicroTileModeForSurfaceType(Gnm::MicroTileMode *outMicroTileMode, SurfaceType surfaceType);

		/**
		 * @brief Determines the proper tile mode for a surface, given a variety of surface attributes.
		 *
		 * @param[in] minGpuMode            The minimum GPU mode with which this surface must be compatible. This value influences various tiling computations.
		 * @param[out] outTileMode			If an appropriate tiling mode can be found for the specified inputs, it will be written here. This pointer must not be <c>NULL</c>.
		 * @param[in] arrayMode				Indicates very broadly how the surface should be tiled (such as linear, 1D/microtiled, or 2D/macrotiled).
		 *								    Note that if a linear mode is indicated, all other parameters will be overridden, and the output will always be a linear tile mode.
		 * @param[in] flags					A variety of flags describing how the surface will be used. Use getFlagsForSurfaceType() to generate the correct
		 *								    flags for most surface types.
		 * @param[in] surfaceFormat			The data format of a single surface element.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel, for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 * @param[in] microTileMode			Another overall indicator of how the surface should be tiled.
		 *
		 * @return						    A status code from GpuAddress::Status.
		 */
		int32_t computeSurfaceTileMode(Gnm::GpuMode minGpuMode, Gnm::TileMode *outTileMode, Gnm::ArrayMode arrayMode, SurfaceFlags flags, Gnm::DataFormat surfaceFormat, uint32_t numFragmentsPerPixel, Gnm::MicroTileMode microTileMode);

		/**
		 * @deprecated This function now takes a Gnm::GpuMode argument.
		 * @brief Determines the proper tile mode for a surface, given a variety of surface attributes.
		 *
		 * @param[out] outTileMode			If an appropriate tiling mode can be found for the specified inputs, it will be written here. This pointer must not be <c>NULL</c>.
		 * @param[in] arrayMode				Indicates very broadly how the surface should be tiled (such as linear, 1D/microtiled, or 2D/macrotiled).
		 *								    Note that if a linear mode is indicated, all other parameters will be overridden, and the output will always be a linear tile mode.
		 * @param[in] flags					A variety of flags describing how the surface will be used. Use getFlagsForSurfaceType() to generate the correct
		 *								    flags for most surface types.
		 * @param[in] surfaceFormat			The data format of a single surface element.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel, for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 * @param[in] microTileMode			Another overall indicator of how the surface should be tiled.
		 *
		 * @return						    A status code from GpuAddress::Status.
		 */
		
		int32_t computeSurfaceTileMode(Gnm::TileMode *outTileMode, Gnm::ArrayMode arrayMode, SurfaceFlags flags, Gnm::DataFormat surfaceFormat, uint32_t numFragmentsPerPixel, Gnm::MicroTileMode microTileMode);

		/**
		 * @brief Determines the proper tile mode for a surface, given a single, simplified "surface type" code.
		 *
		 * @param[in] minGpuMode            The minimum GPU mode with which this surface must be compatible. This value influences various tiling computations.
		 * @param[out] outTileMode			If an appropriate tiling mode can be found for the specified inputs, it will be written here. This pointer must not be <c>NULL</c>.
		 * @param[in] surfaceType			Indicates the type of surface to evaluate.
		 * @param[in] surfaceFormat			The data format of a single surface element.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel, for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return						    A status code from GpuAddress::Status.
		 */
		int32_t computeSurfaceTileMode(Gnm::GpuMode minGpuMode, Gnm::TileMode *outTileMode, SurfaceType surfaceType, Gnm::DataFormat surfaceFormat, uint32_t numFragmentsPerPixel);

		/**
		 * @deprecated This function now takes a Gnm::GpuMode argument.
		 * @brief Determines the proper tile mode for a surface, given a single, simplified "surface type" code.
		 *
		 * @param[out] outTileMode			If an appropriate tiling mode can be found for the specified inputs, it will be written here. This pointer must not be <c>NULL</c>.
		 * @param[in] surfaceType			Indicates the type of surface to consider.
		 * @param[in] surfaceFormat			The data format of a single surface element.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel, for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return						    A status code from GpuAddress::Status.
		 */
		
		int32_t computeSurfaceTileMode(Gnm::TileMode *outTileMode, SurfaceType surfaceType, Gnm::DataFormat surfaceFormat, uint32_t numFragmentsPerPixel);

		/**
		 * @brief Determines the macro tile mode for a surface, according to its tile mode and surface element format.
		 *
		 * Generally, this is necessary only for internal use of the GpuAddress library.
		 *
		 * @param[out] outMacroTileMode		Receives the macro tile mode of the surface if the operation is successful.
		 * @param[in] tileMode				The tile mode of the surface.
		 * @param[in] bitsPerElement		The size of a single surface element in bits, as reported by DataFormat::getTotalBitsPerElement().
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return                          A status code from GpuAddress::Status.
		 */
		int32_t computeSurfaceMacroTileMode(Gnm::MacroTileMode *outMacroTileMode, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel);

		/**
		 * @brief Computes the base address and size of the surface data for the surface at the specified array slice (or mip level) of a Texture.
		 *
		 * @param[out] outSurfaceOffset	If non-<c>NULL</c>, the byte offset of the specified surface will be stored here. This offset is relative to
		 *							    the texture's base or mip-base address, depending on the value of mipLevel.
		 * @param[out] outSurfaceSize	If non-<c>NULL</c>, the size (in bytes) of the specified surface will be stored here.
		 * @param[in] texture			The texture whose dimensions and metadata should be used to compute the requested offset and size.
		 * @param[in] mipLevel			Index of the mip level to query. A value of <c>0</c> corresponds to the base level (the largest mip.) This value must be less than the texture's
		 *                              maximum possible mip level, as dictated by its dimensions and TextureType.
		 * @param[in] arraySlice		For texture arrays, specify the array index of the query. For cubemaps, pass the index of the desired face (see the <c>CubemapFace</c> enum).
		 *							    For cubemap arrays, specify <c><i>N</i>*6+<i>F</i></c>, in which <c><i>N</i></c> is the cubemap index and <c><i>F</i></c> is the face index. For all other textures, specify <c>0</c>.
		 *                              This value must be less than the texture's total array slice count.
		 *
		 * @return					    A status code from GpuAddress::Status.
		 */
		int32_t computeTextureSurfaceOffsetAndSize(uint64_t *outSurfaceOffset, uint64_t *outSurfaceSize, const Gnm::Texture *texture, uint32_t mipLevel, uint32_t arraySlice);

		/**
		 * @brief Computes the base address and size of the surface data for the surface at the specified array slice of a RenderTarget.
		 *	
		 * @param[out] outSurfaceOffset	If non-<c>NULL</c>, the byte offset of the specified surface will be stored here.  This offset is relative to
		 *							    the texture's base or mip-base address, depending on the value of mipLevel.
		 * @param[out] outSurfaceSize	If non-<c>NULL</c>, the size (in bytes) of the specified surface will be stored here.
		 * @param[in] target			The render target from which dimensions and metadata are used to compute the requested offset and size.
		 * @param[in] arraySlice		For texture arrays, specify the array index of the query. For cubemaps, pass the index of the desired face (see the <c>CubemapFace</c> enum).
		 *							    For cubemap arrays, specify <c><i>N</i>*6+<i>F</i></c>, in which <c><i>N</i></c> is the cubemap index and <c><i>F</i></c> is the face index. For all other textures, specify <c>0</c>.
		 *
		 * @return					    A status code from GpuAddress::Status.
		 */
		int32_t computeRenderTargetSurfaceOffsetAndSize(uint64_t *outSurfaceOffset, uint64_t *outSurfaceSize, const Gnm::RenderTarget *target, uint32_t arraySlice);

		/**
		 * @brief Computes the base address and size of the surface data for the surface at the specified array slice of a DepthRenderTarget's depth surface.
		 *
		 * @param[out] outSurfaceOffset	If non-<c>NULL</c>, the byte offset of the specified surface will be stored here. This offset is relative to
		 *							    the texture's base or mip-base address, depending on the value of <c>mipLevel</c>.
		 * @param[out] outSurfaceSize	If non-<c>NULL</c>, the size (in bytes) of the specified surface will be stored here.
		 * @param[in] depthTarget		The depth render target from which dimensions and metadata are used to compute the requested offset and size.
		 * @param[in] arraySlice		For texture arrays, specify the array index of the query. For cubemaps, pass the index of the desired face (see the <c>CubemapFace</c> enum).
		 *							    For cubemap arrays, specify <c><i>N</i>*6+<i>F</i></c>, in which <c><i>N</i></c> is the cubemap index and <c><i>F</i></c> is the face index. For all other textures, specify <c>0</c>.
		 *
		 * @return					    A status code from GpuAddress::Status.
		 */
		int32_t computeDepthSurfaceOffsetAndSize(uint64_t *outSurfaceOffset, uint64_t *outSurfaceSize, const Gnm::DepthRenderTarget *depthTarget, uint32_t arraySlice);

		/**
		 * @brief Computes the base address and size of the surface data for the surface at the specified array slice of the stencil surface of the specified DepthRenderTarget.
		 *
		 * @param[out] outSurfaceOffset	If non-<c>NULL</c>, the byte offset of the specified surface will be stored here. This offset is relative to
		 *							    the texture's base or mip-base address, depending on the value of mipLevel.
		 * @param[out] outSurfaceSize	If non-<c>NULL</c>, the size (in bytes) of the specified surface will be stored here.
		 * @param[in] depthTarget		The depth render target from which dimensions and metadata are used to compute the requested offset and size.
		 * @param[in] arraySlice		For texture arrays, specify the array index of the query. For cubemaps, pass the index of the desired face (see the <c>CubemapFace</c> enum).
		 *							    For cubemap arrays, specify <c><i>N</i>*6+<i>F</i></c>, in which <c><i>N</i></c> is the cubemap index and <c><i>F</i></c> is the face index. For all other textures, specify <c>0</c>.
		 *
		 * @return					    A status code from GpuAddress::Status.
		 */
		int32_t computeStencilSurfaceOffsetAndSize(uint64_t *outSurfaceOffset, uint64_t *outSurfaceSize, const Gnm::DepthRenderTarget *depthTarget, uint32_t arraySlice);

		/**
		 * @brief Describes how to tile a single surface.
		 *
		 * A <i>surface</i> in this context is one mip level of one array slice of a single Texture, or one slice of one RenderTarget slice.
		 */
		class TilingParameters
		{
		public:
			Gnm::TileMode m_tileMode;			///< The surface tile mode.
			Gnm::GpuMode m_minGpuMode;          ///< The minimum GPU mode with which this surface must be compatible. This value influences various tiling computations.

			uint32_t m_linearWidth;				///< Logical, linear width of the surface in pixels/texels (before alignment/padding). If <c><i>m_mipLevel</i></c> > 0, this should be the width of the specified mip level (not the base level).
			uint32_t m_linearHeight;			///< Logical, linear height of the surface in pixels/texels (before alignment/padding). If <c><i>m_mipLevel</i></c> > 0, this should be the height of the specified mip level (not the base level).
			uint32_t m_linearDepth;				///< For volume textures, the logical, linear depth of the texture in pixels/texels (before alignment/padding). If <c><i>m_mipLevel</i></c> > 0, this should be the depth of the specified mip level (not the base level). For non-volume surfaces, specify 1.
			uint32_t m_numFragmentsPerPixel;	///< The actual number of fragments stored per pixel if the surface represents a multi-sampled render target. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
			uint32_t m_baseTiledPitch;			///< If <c><i>m_mipLevel</i></c> is greater than 0, this must be the pitch in pixels/texels of the base mip level (level 0) when tiled, as determined by computeSurfaceInfo().
												///< If <c><i>m_mipLevel</i></c> is 0 and <c><i>m_baseTiledPitch</i></c> is 0, the GpuAddress library will compute the minimum valid pitch for this surface.
												///< If <c><i>m_mipLevel</i></c> is 0 and <c><i>m_baseTiledPitch</i></c> is a valid pitch greater than 0, it will be used as the pitch for mip level 0.
												///< If <c><i>m_mipLevel</i></c> is 0 and <c><i>m_baseTiledPitch</i></c> is an invalid pitch greater than 0, an error will be generated.
			uint32_t m_mipLevel;				///< Level 0 is the base image.
			uint32_t m_arraySlice;				///< For texture arrays, set to the array index of the slice to tile. For cubemaps, set to the cubemap face (see the <c>CubemapFace</c> enum). For cubemap arrays, set to slice*6+face. Otherwise, set to 0.
			SurfaceFlags m_surfaceFlags;		///< Set the appropriate bits for the surface being tiled/detiled.
			uint32_t m_bitsPerFragment;			///< The number of bits per surface fragment. For BCn block-compressed surfaces, this must be 1/16th of the block size. This field is usually set to surfaceFormat.getTotalBitsPerElement() / surfaceFormat.getTexelsPerElement().
			bool m_isBlockCompressed;			///< Set to <c>true</c> if surfaceFormat.getTexelsPerElement() > 1; otherwise, set to <c>false</c>. The name of this field is misleading. Historically, only BCn block-compressed formats met this condition in Gnm, but now it covers formats such as <c>kSurfaceFormat1</c> and <c>kSurfaceFormatGB_GR</c>.
			uint8_t m_tileSwizzleMask;			///< Tile swizzle mask for this surface, as reported by getTileSwizzleMask() (or similar). This field only affects the location of individual elements within a surface, and can be safely set to zero when filling in a TilingParameters object from scratch.

			/**
			 * @brief Initializes a TilingParameters object from an existing Texture object.
			 *
			 * A Texture may be composed of many surfaces (mips, array slices, etc.).
			 *
			 * @param[in] texture		The Texture to use as the source for tiling information. This pointer must not be <c>NULL</c>.
			 * @param[in] mipLevel		The index of the mip level to set up tiling for. A value of 0 corresponds to the base level (the largest mip).
			 * @param[in] arraySlice	For texture arrays, specify the array index of the query. For cubemaps, pass the index of the desired face (see the <c>CubemapFace</c> enum).
			 *							For cubemap arrays, specify <c><i>N</i>*6+<i>F</i></c>, in which <c><i>N</i></c> is the cubemap index and <c><i>F</i></c> is the face index. For all other textures, specify <c>0</c>.
			 *
			 * @return					A status code from GpuAddress::Status.
			 */
			int32_t initFromTexture(const Gnm::Texture *texture, uint32_t mipLevel, uint32_t arraySlice);

			/**
			 * @brief Initializes a TilingParameters object from an existing RenderTarget object.
			 *
			 * A RenderTarget may be composed of many surfaces if it has multiple array slices.
			 *
			 * @param[in] target		The RenderTarget to use as the source for tiling information. This pointer must not be <c>NULL</c>.
			 * @param[in] arraySlice	The index of the array slice for which this function sets up tiling. For non-array surfaces, specify <c>0</c>.
			 *
			 * @return					A status code from GpuAddress::Status.
			 */
			int32_t initFromRenderTarget(const Gnm::RenderTarget *target, uint32_t arraySlice);

			/**
			 * @brief Initializes a TilingParameters object from an existing DepthRenderTarget object's depth surface.
			 *
			 * A DepthRenderTarget object's depth surface may have multiple array slices.
			 *
			 * @param[in] target		The DepthRenderTarget that supplies the depth surface to use as the source for tiling information. This pointer must not be <c>NULL</c>.
			 * @param[in] arraySlice	The index of the array slice for which this function sets up tiling. For non-array surfaces, specify <c>0</c>.
			 *
			 * @return					A status code from GpuAddress::Status.
			 */
			int32_t initFromDepthSurface(const Gnm::DepthRenderTarget *target, uint32_t arraySlice);

			/**
			 * @brief Initializes a TilingParameters object from an existing DepthRenderTarget object's stencil surface.
			 *
			 * A DepthRenderTarget object's stencil surface may have multiple array slices.
			 *
			 * @param[in] target		The DepthRenderTarget that supplies the stencil surface to use as the source for tiling information. This pointer must not be <c>NULL</c>.
			 * @param[in] arraySlice	The index of the array slice for which this function sets up tiling. For non-array surfaces, specify <c>0</c>.
			 *
			 * @return					A status code from GpuAddress::Status.
			 */
			int32_t initFromStencilSurface(const Gnm::DepthRenderTarget *target, uint32_t arraySlice);
		};


		/**
		 * @brief Computes the size of a surface before it has been tiled.
		 *
		 * @param[out] outSize			Receives the required size of the untiled surface if the operation was successful.
		 * @param[out] outAlign			Receives the required surface alignment of the untiled surface if the operation was successful.
		 * @param[in] tp				A description of the surface for which this function computes size. Note that the <c><i>m_tileMode</i></c> field
		 *							    will be ignored. Only the mip level specified in <c><i>tp->m_mipLevel</i></c> and the array slice specified
		 *							    in <c><i>tp->m_arraySlice</i></c> are considered; to compute the total size of all mip levels and
		 *							    array slices, use computeTotalUntiledTextureSize() instead of this function. This pointer must not be <c>NULL</c>.
		 *
		 * @return					    A status code from GpuAddress::Status.
		 *
		 * @sa computeTotalUntiledTextureSize()
		 */
		int32_t computeUntiledSurfaceSize(uint64_t *outSize, Gnm::AlignmentType *outAlign, const TilingParameters *tp);

		/**
		 * @brief Computes the size of a surface after it has been tiled.
		 *
		 * @param[out] outSize			Receives the required size of the tiled surface if the operation was successful.
		 * @param[out] outAlign			Receives the required surface alignment of the tiled surface if the operation was successful,.
		 * @param[in] tp				A description of the surface for which this function computes size. Only the mip level specified in <c><i>tp->m_mipLevel</i></c>
		 *								and the array slice specified in <c><i>tp->m_arraySlice</i></c> are considered; to compute
		 *								the total size of all mip levels and array slices, use computeTotalTiledTextureSize() instead of this function.
		 *								This pointer must not be <c>NULL</c>.
		 *
		 * @return						A status code from GpuAddress::Status.
		 *
		 * @sa computeTotalTiledTextureSize()
		 */
		int32_t computeTiledSurfaceSize(uint64_t *outSize, Gnm::AlignmentType *outAlign, const TilingParameters *tp);

		/**
		 * @brief Computes the total size of a texture (including multiple mip levels and array slices) before it has been tiled.
		 *
		 * All alignment and padding restrictions will be ignored, mip levels will be tightly packed, and so on.
		 * The returned size will include all mip levels up to <c><i>texture</i></c>->getLastMipLevel(), and all array slices up to <c><i>texture</i></c>->getLastArraySliceIndex().
		 *
		 * @param[out] outSize		Receives the required size of the untiled surface if the operation was successful.
		 * @param[out] outAlign		Receives the required surface alignment of the untiled surface if the operation was successful.
		 * @param[in] texture		The texture for which this function computes size. This argument must not be <c>NULL</c>.
		 *
		 * @return					A status code from GpuAddress::Status.
		 *
		 */
		int32_t computeTotalUntiledTextureSize(uint64_t *outSize, Gnm::AlignmentType *outAlign, const Gnm::Texture *texture);

		/**
		 * @brief Computes the total size of a texture after it has been tiled.
		 *
		 * The function takes into account multiple mip levels and array slices, and the returned size includes all relevant alignment and padding restrictions.
		 * The returned size will also include all mip levels up to <c><i>texture</i></c>->getLastMipLevel(), and all array slices up to <c><i>texture</i></c>->getLastArraySliceIndex().
		 *
		 * @param[out] outSize			Receives the required size of the tiled surface if the operation was successful. This argument must not be <c>NULL</c>.
		 * @param[out] outAlign			Receives the required surface alignment of the tiled surface if the operation was successful. This argument must not be <c>NULL</c>.
		 * @param[in] texture			The texture whose size is to be determined. This argument must not be <c>NULL</c>.
		 *
		 * @return						A status code from GpuAddress::Status.
		 *
		 */
		int32_t computeTotalTiledTextureSize(uint64_t *outSize, Gnm::AlignmentType *outAlign, const Gnm::Texture *texture);

		/**
		 * @brief Computes the tiled byte offset for the specified element in a surface.
		 *
		 * Note that this function is <i>not</i> optimized for repeat calls. To convert an entire surface between
		 * tiled and untiled formats, use the tileSurface() and detileSurface() wrappers instead.
		 *
		 * @param[out] outTiledByteOffset	Receives the byte offset of the specified element in a tiled surface buffer. This pointer must not be <c>NULL</c>.
		 * @param[in] tp					The surface tiling parameters.
		 * @param[in] x						The X-coordinate of the element to locate. For surface formats with multiple texels per element, specify this value in units of elements rather than individual texels.
		 * @param[in] y						The Y-coordinate of the element to locate. For surface formats with multiple texels per element, specify this value in units of elements rather than individual texels.
		 * @param[in] z						The Z-coordinate of the element to locate. For non-volume surfaces, specify <c>0</c>.
		 * @param[in] fragmentIndex			The index of the sub-pixel fragment to locate. For non-MSAA surfaces, specify <c>0</c>.
		 *
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa    computeLinearElementByteOffset()
		 */
		int32_t computeTiledElementByteOffset(uint64_t *outTiledByteOffset, const TilingParameters *tp, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex);

		/**
		 * @brief Computes the linear (untiled) byte offsets for the specified element in a surface.
		 *
		 * Note that this function is <i>not</i> optimized for repeat calls. To convert an entire surface between
		 * tiled and untiled formats, use the tileSurface() and detileSurface() wrappers instead of this function.
		 *
		 * @param[out] outUntiledByteOffset		Receives the byte offset of the specified element in an untiled surface buffer. This pointer must not be <c>NULL</c>.
		 * @param[in] x							The X-coordinate of the pixel containing the element to locate. For surface formats with multiple texels per element, specify this value in units of elements rather than individual texels.
		 * @param[in] y							The Y-coordinate of the pixel containing the element to locate. For surface formats with multiple texels per element, specify this value in units of elements rather than individual texels.
		 * @param[in] z							The Z-coordinate of the pixel containing the element to locate. For non-3D surfaces, specify <c>0</c>.
		 * @param[in] fragmentIndex				The index of the sub-pixel fragment to locate. For non-MSAA surfaces, specify <c>0</c>.
		 * @param[in] pitch						The pitch of the untiled surface data in pixels (for render targets) or elements (for textures.)
		 * @param[in] slicePitch				The size of one Z-slice of the untiled pixel data in pixels (for render targets) or elements (for textures.)
		 * @param[in] bitsPerElement			The size of one surface element in bits, as reported by the DataFormat::getTotalBitsPerElement() function.
		 * @param[in] numFragmentsPerPixel		The number of fragments per pixel for MSAA surfaces. For single-sampled surfaces, specify <c>1</c>. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return								A status code from GpuAddress::Status.
		 *
		 * @sa    computeTiledElementByteOffset()
		 */
		int32_t computeLinearElementByteOffset(uint64_t *outUntiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex,
			uint32_t pitch, uint32_t slicePitch, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel);

		/**
			* @brief Tiles a subregion of a surface as tiling arguments specify.
			*
			* Internally, this function chooses an appropriate <c>Tiler</c> class (<c>TilerLinear</c>, <c>Tiler1d</c>, or <c>Tiler2d</c>) and calls its tileSurfaceRegion() method.
			* Only a single mip level (<c><i>tp.m_mipLevel</i></c>) will be processed.
			*
			* @param[out] outTiledSurface		The base address of the destination surface that is to receive the tiled surface data. Use computeTiledSurfaceSize() to determine the minimum required buffer size.
			* @param[in] untiledSurface			Pointer to the source untiled surface data that this function reads. The size of this buffer must be at least as large as
			*								    <c>(<i>destRegion</i>-><i>m_back</i> - <i>destRegion</i>-><i>m_front</i>)*<i>sourceSlicePitch</i>*(tp-><i>m_bitsPerElement</i>/8)</c>.
			* @param[in] tp						The tiling parameters.
			* @param[in] destRegion				Describes the bounds of the region in the destination surface to which this function writes tiled data.
			*									The region must not extend beyond the bounds of the destination surface.
			*									If the region is empty (left==right, top==bottom, or front==back), this function returns immediately.
			* @param[in] sourcePitch		    The pitch of the <c><i>untiledSurface</i></c> data, in pixels (for render targets) or in elements (for textures). The value must be at least as large as the width (right-left) of the <c><i>destRegion</i></c> argument.
			* @param[in] sourceSlicePitch	    The size of one Z-slice of the <c><i>untiledSurface</i></c> data, specified in pixels (for render targets) or specified in elements (for textures). This value must be at least as large as the width (right-left) of the <c><i>destRegion</i></c> argument multiplied by its height (bottom-top).
			*
			* @return							A status code from GpuAddress::Status.
			*
			* @sa detileSurface(), tileSurface(), detileSurfaceRegion()
			*/
		int32_t tileSurfaceRegion(void *outTiledSurface, const void *untiledSurface, const TilingParameters *tp, const SurfaceRegion *destRegion, uint32_t sourcePitch, uint32_t sourceSlicePitch);

		/**
			* @brief Detiles a subregion of a surface as tiling arguments specify.
			*
			* Internally, this function chooses an appropriate <c>Tiler</c> class (<c>TilerLinear</c>, <c>Tiler1d</c>, or <c>Tiler2d</c>) and calls its detileSurfaceRegion() method. Only a single mip level (<c><i>tp.m_mipLevel</i></c>) will be processed.
			*
			* @param[out] outUntiledSurface		The destination buffer that is to receive the de-tiled surface data. The size of this buffer must be at least as large as
			*									<c>(<i>srcRegion</i>-><i>m_back</i> - <i>srcRegion</i>-><i>m_front</i>)*<i>destSlicePitch</i>*(tp-><i>m_bitsPerElement</i>/8)</c>.
			* @param[in] tiledSurface			The base address of the source-tiled surface data. Use the computeTiledSurfaceSize() function to determine how much data this 
			*									function will read from the <c>tiledSurface</c> buffer. This pointer must not be <c>NULL</c>.
			* @param[in] tp						The tiling parameters.
			* @param[in] srcRegion				Describes the bounds of the region in the source surface from which this function reads tiled data.
			*									This region must not extend beyond the bounds of the destination buffer.
			*									If the region is empty (left==right, top==bottom, or front==back), this function returns immediately.
			* @param[in] destPitch				The pitch of the <c><i>outUntiledSurface</i></c> data, specified in pixels (for render targets) or specified in elements (for textures.) This value must be at least as large as the width (right-left) of the <c><i>srcRegion</i></c> argument.
			* @param[in] destSlicePitch			The size of one Z-slice of the <c><i>outUntiledSurface</i></c> data, specified in pixels (for render targets) or specified in elements (for textures.) This value must be at least as large as the width (right-left) of the <c><i>srcRegion</i></c> argument multiplied by its height (bottom-top).
			*
			* @return							A status code from GpuAddress::Status.
			*
			* @sa detileSurface(), tileSurface(), tileSurfaceRegion()
			*/
		int32_t detileSurfaceRegion(void *outUntiledSurface, const void *tiledSurface, const TilingParameters *tp, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitch);

		/**
		 * @brief Converts untiled surface data to tiled surface data as tiling arguments specify.
		 *
		 * Internally, this function chooses an appropriate <c>Tiler</c> class (<c>TilerLinear</c>, <c>Tiler1d</c>, or <c>Tiler2d</c>) and calls its tileSurface() method. Only a single mip level (<c><i>tp.m_mipLevel</i></c>) will be processed.
		 *
		 * @param[out] outTiledSurface			Receives the tiled surface data. Use the computeTiledSurfaceSize() function to determine the minimum 
		 *										required size of this buffer. This pointer must not be <c>NULL</c>.
		 * @param[in] untiledSurface			Pointer to the source untiled surface data that this function reads. Use the computeUntiledSurfaceSize() function to determine how much data this 
		 *										function will read from the <c>untiledSurface</c> buffer. This pointer must not be <c>NULL</c>.
		 * @param[in] tp						The tiling parameters.
		 *
		 * @return								A status code from GpuAddress::Status.
		 *
		 * @sa     tileSurfaceRegion(), detileSurface(), computeTiledSurfaceSize(), computeUntiledSurfaceSize()
		 */
		int32_t tileSurface(void *outTiledSurface, const void *untiledSurface, const TilingParameters *tp);

		/**
		 * @brief Converts tiled surface data to untiled surface data as tiling arguments specify.
		 *
		 * Internally, this function chooses an appropriate <c>Tiler</c> class (<c>TilerLinear</c>, <c>Tiler1d</c>, or <c>Tiler2d</c>) and calls its detileSurface() method. Only a single mip level (<c><i>tp.m_mipLevel</i></c>) will be processed.
		 *
		 * @param[out] outUntiledSurface		Pointer to which this function writes untiled surface data. Call the computeUntiledSurfaceSize() function to determine the minimum 
		 *										required size of this buffer. This pointer must not be <c>NULL</c>.
		 * @param[in] tiledSurface				Pointer to the source tiled surface data that this function reads. Use the computeTiledSurfaceSize() function to determine how much data this 
		 *										function will read from the <c>tiledSurface</c> buffer. This pointer must not be <c>NULL</c>.  
		 * @param[in] tp						The tiling parameters.
		 *
		 * @return								A status code from GpuAddress::Status.
		 *
		 * @sa     detileSurfaceRegion(), tileSurface(), computeTiledSurfaceSize(), computeUntiledSurfaceSize()
		 */
		int32_t detileSurface(void *outUntiledSurface, const void *tiledSurface, const TilingParameters *tp);

		/////////////////////////////////////////////////////////
		// Buffer swizzling functions
		/////////////////////////////////////////////////////////

		/**
		 * @brief Converts buffer data from linear format to element-level swizzled format.
		 *
		 * @param[in] targetGpuMode     The GPU mode for which to create swizzled data.
		 * @param[out] outSwizzledData	Receives the swizzled data. Use the computeSwizzledBufferSize() function to determine the minimum
		 *								required buffer size. The data must not overlap <c><i>linearData</i></c>, and this pointer must not be <c>NULL</c>.
		 * @param[in] linearData		Pointer from which this function reads source linear data. The amount of data that will be read is
		 *								<c><i>elemStride</i></c>*<c><i>numElements</i></c> bytes.
		 *								The data must not overlap <c><i>outSwizzledData</i></c>, and this pointer must not be <c>NULL</c>.
		 * @param[in] elemStride		The size of a single buffer element, expressed as a number of bytes.
		 * @param[in] numElements		The number of elements in the buffer.
		 * @param[in] swizzleSize		The number of contiguous bytes from each buffer element to store contiguously in the swizzled data. This size should
		 *								match the granularity at which shaders are to access the data. For example, if the shader is loading
		 *								four bytes per thread, set <c><i>swizzleSize</i></c> to <c>kBufferSwizzleElementSize4</c>.
		 *								The size of a single buffer element must be exactly divisible by <c><i>swizzleSize</i></c>; otherwise, the results will be undefined.
		 * @param[in] swizzleStride		The number of consecutive buffer elements to swizzle in a block from AoS to SoA. For maximum performance, the
		 *								product of the <c><i>swizzleStride</i></c> and the <c><i>swizzleSize</i></c> should be an exact multiple of the cache line size (64 bytes.)
		 *		 
		 * @return						A status code from GpuAddress::Status.
		 *
		 * @sa    Gnm::Buffer::setSwizzle(), computeSwizzledBufferSize(), deswizzleBufferData()
		 */
		int32_t swizzleBufferData(Gnm::GpuMode targetGpuMode, void *outSwizzledData, const void *linearData, uint32_t elemStride, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride);

		/**
		 * @brief Converts buffer data from linear format to element-level swizzled format.
		 *
		 * @deprecated This function now takes a Gnm::GpuMode argument.
		 * @param[out] outSwizzledData	Receives the swizzled data. Use the computeSwizzledBufferSize() function to determine the required 
		 *								size of the buffer. The data must not overlap <c><i>linearData</i></c>, and the pointer must not be <c>NULL</c>.
		 * @param[in] linearData		Pointer from which this function reads source linear data. The amount of data that will be read is
		 *								<c><i>elemStride</i></c>*<c><i>numElements</i></c> bytes.
		 *								The data must not overlap <c><i>outSwizzledData</i></c>, and the pointer must not be <c>NULL</c>.
		 * @param[in] elemStride		The size of a single buffer element, expressed as a number of bytes.
		 * @param[in] numElements		The number of elements in the buffer.
		 * @param[in] swizzleSize		The number of contiguous bytes from each buffer element to store contiguously in the swizzled data. This size should
		 *								match the granularity at which shaders are to access the data. For example, if the shader is loading
		 *								four bytes per thread, set <c><i>swizzleSize</i></c> to <c>kBufferSwizzleElementSize4</c>.
		 *								The size of a single buffer element must be exactly divisible by <c><i>swizzleSize</i></c>; otherwise, the results will be undefined.
		 * @param[in] swizzleStride		The number of consecutive buffer elements to swizzle in a block from AoS to SoA. For maximum performance, the
		 *								product of the <c><i>swizzleStride</i></c> and the <c><i>swizzleSize</i></c> should be an exact multiple of the cache line size (64 bytes.)
		 *		 
		 * @return						A status code from GpuAddress::Status.
		 *
		 * @sa    Gnm::Buffer::setSwizzle(), computeSwizzledBufferSize(), deswizzleBufferData()
		 */
		
		int32_t swizzleBufferData(void *outSwizzledData, const void *linearData, uint32_t elemStride, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride);

		/**
		 * @brief Converts buffer data from element-level swizzled format to linear format.
		 *
		 * @param[in] targetGpuMode     The GPU mode for which this method creates de-swizzled data.
		 * @param[out] outLinearData	Receives the linear data. The buffer must be sufficient to receive 
		 *								<c><i>elemStride</i></c>*<c><i>numElements</i></c> bytes of data.
		 *								The data must not overlap <c><i>SwizzledData</i></c>, and this pointer must not be <c>NULL</c>.
		 * @param[in] swizzledData		Pointer from which this function reads source swizzled data. Use the computeSwizzledBufferSize() function to determine 
		 *								the amount of data that will be read. The data must not overlap <c><i>outLinearData</i></c>, and this pointer must not be <c>NULL</c>.
		 * @param[in] elemStride		The size of a single buffer element, expressed as a number of bytes.
		 * @param[in] numElements		The number of elements in the buffer.
		 * @param[in] swizzleSize		The number of contiguous bytes from each buffer element that <c><i>swizzledData</i></c> stores contiguously, as reported by sce::Gnm::Buffer:: getSwizzleElementSize(). This size should
		 *								match the granularity at which shaders are to access the data. For example, if the shader is loading
		 *								four bytes per thread, set <c><i>swizzleSize</i></c> to <c>kBufferSwizzleElementSize4</c>.
		 *								The size of a single buffer element must be exactly divisible by <c><i>swizzleSize</i></c>; otherwise, the results will be undefined.
		 * @param[in] swizzleStride		The number of consecutive buffer elements that <c><i>swizzledData</i></c> swizzled together in a block from AoS to SoA, as reported by sce::Gnm::Buffer:: getSwizzleStride(). For maximum performance, the
		 *								product of the <c><i>swizzleStride</i></c> and the <c><i>swizzleSize</i></c> should be an exact multiple of the cache line size (64 bytes).
		 *		 
		 * @return						A status code from GpuAddress::Status.
		 *
		 * @sa    Gnm::Buffer::setSwizzle(), computeSwizzledBufferSize(), swizzleBufferData()
		 */
		int32_t deswizzleBufferData(Gnm::GpuMode targetGpuMode, void *outLinearData, const void *swizzledData, uint32_t elemStride, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride);

		/**
		 * @deprecated This function now takes a Gnm::GpuMode argument.
		 * @brief Converts buffer data from element-level swizzled format to linear.
		 *
		 * @param[out] outLinearData	Receives the linear data. The buffer must be sufficient to receive
		 *								<c><i>elemStride</i></c>*<c><i>numElements</i></c> bytes of data.
		 *								The data must not overlap <c><i>SwizzledData</i></c>, and this pointer must not be <c>NULL</c>.
		 * @param[in] swizzledData		Pointer from which this function reads source swizzled data. Use the computeSwizzledBufferSize() function to determine 
		 *								the amount of data that will be read. The data must not overlap <c><i>outLinearData</i></c>, and this pointer must not be <c>NULL</c>.
		 * @param[in] elemStride		The size of a single buffer element, expressed as a number of bytes.
		 * @param[in] numElements		The number of elements in the buffer.
		 * @param[in] swizzleSize		The number of contiguous bytes from each buffer element to store contiguously in the swizzled data. This size should
		 *								match the granularity at which shaders are to access the data. For example, if the shader is loading
		 *								fou bytes per thread, set <c><i>swizzleSize</i></c> to <c>kBufferSwizzleElementSize4</c>.
		 *								The size of a single buffer element must be exactly divisible by <c><i>swizzleSize</i></c>; otherwise, the results will be undefined.
		 * @param[in] swizzleStride		The number of consecutive buffer elements to swizzle in a block from AoS to SoA. For maximum performance, the
		 *								product of the <c><i>swizzleStride</i></c> and the <c><i>swizzleSize</i></c> should be an exact multiple of the cache line size (64 bytes.)
		 *		 
		 * @return						A status code from GpuAddress::Status.
		 *
		 * @sa    Gnm::Buffer::setSwizzle(), computeSwizzledBufferSize(), swizzleBufferData()
		 */
		
		int32_t deswizzleBufferData(void *outLinearData, const void *swizzledData, uint32_t elemStride, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride);

		/**
		 * @brief Computes the size of a swizzled buffer.
		 *
		 * A swizzled buffer often requires more space than its linear equivalent, due to extra padding.
		 *
		 * @param[in] targetGpuMode         The GPU mode for which this function creates swizzled data.
		 * @param[out] outSizeBytes			Receives the size of the swizzled buffer in bytes if the operation was successful. This pointer must not be <c>NULL</c>.
		 * @param[in] elementSizeInBytes	The size of a single buffer element, expressed as a number of bytes.
		 * @param[in] numElements			The total number of elements in the buffer.
		 * @param[in] swizzleSize			The number of contiguous bytes from each buffer element to store contiguously in the swizzled data. This sizes hould
		 *									match the granularity at which shaders are to access the data. For example, if the shader is loading
		 *									four bytes per thread, set <c><i>swizzleSize</i></c> to <c>kBufferSwizzleElementSize4</c>.
		 *									The size of a single buffer element must be exactly divisible by <c><i>swizzleSize</i></c>; otherwise, the results will be undefined.
		 * @param[in] swizzleStride			The number of consecutive buffer elements to swizzle in a block from AoS to SoA. For maximum performance, the
		 *									product of the <c><i>swizzleStride</i></c> and the <c><i>swizzleSize</i></c> should be an exact multiple of the cache line size (64 bytes.)
		 *		 
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa Gnm::Buffer::setSwizzle()
		 */
		int32_t computeSwizzledBufferSize(Gnm::GpuMode targetGpuMode, uint64_t *outSizeBytes, uint32_t elementSizeInBytes, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride);

		/**
		 * @brief Computes the size of a swizzled buffer.
		 *
		 * A swizzled buffer often requires more space than its linear equivalent, due to extra padding.
		 *
		 * @deprecated This function now takes a Gnm::GpuMode argument.
		 * @param[out] outSizeBytes			Receives the size of the swizzled buffer in bytes if the operation was successful. This pointer must not be <c>NULL</c>.
		 * @param[in] elementSizeInBytes	The size of a single buffer element, expressed as a number of bytes.
		 * @param[in] numElements			The total number of elements in the buffer.
		 * @param[in] swizzleSize			The number of contiguous bytes from each buffer element to store contiguously in the swizzled data. This size should
		 *									match the granularity at which shaders are to access the data. For example, if the shader is loading
		 *									four bytes per thread, set <c><i>swizzleSize</i></c> to <c>kBufferSwizzleElementSize4</c>.
		 *									The size of a single buffer element must be exactly divisible by <c><i>swizzleSize</i></c>; otherwise, the results will be undefined.
		 * @param[in] swizzleStride			The number of consecutive buffer elements to swizzle in a block from AoS to SoA. For maximum performance, the
		 *									product of the <c><i>swizzleStride</i></c> and the <c><i>swizzleSize</i></c> should be an exact multiple of the cache line size (64 bytes.)
		 *		 
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa Gnm::Buffer::setSwizzle()
		 */
		
		int32_t computeSwizzledBufferSize(uint64_t *outSizeBytes, uint32_t elementSizeInBytes, uint32_t numElements, Gnm::BufferSwizzleElementSize swizzleSize, Gnm::BufferSwizzleStride swizzleStride);

		////////////////////////////////////////////////////////////
		// Advanced functions, mainly for internal libgnm use
		////////////////////////////////////////////////////////////

#ifdef _MSC_VER
#pragma warning(push)
#pragma warning(disable:4201) // nameless struct/union
#endif
		/**
		 * @brief Contains the final hardware-compatible values for various surface attributes including padding and alignment restrictions.
		 *
		 * SurfaceInfo objects are passed to computeSurfaceInfo().
		 *
		 * @sa computeSurfaceInfo()
		 */
		class SurfaceInfo
		{
		public:
			uint32_t m_pitch;          ///< The surface pitch in pixels (for render targets) or in elements (for textures.)
			uint32_t m_height;         ///< The surface height in pixels (for render targets) or in elements (for textures.)
			uint32_t m_depth;          ///< The surface depth (<c>>=1</c> for volume textures, or <c>1</c> for all other surfaces.)
			uint8_t m_reserved[4];     ///< Reserved; set to zero.
			uint64_t m_surfaceSize;    ///< The surface size in bytes.
			Gnm::ArrayMode m_arrayMode;///< The actual array mode. Due to complex hardware restrictions, this may be different than the array mode passed by the user in TilingParameters.
			uint32_t m_baseAlign;      ///< The base address alignment.
			uint32_t m_pitchAlign;     ///< The required surface pitch alignment in pixels (for render targets) or elements (for textures.)
			uint32_t m_heightAlign;    ///< The required surface height alignment in pixels (for render targets) or elements (for textures.)
			uint32_t m_depthAlign;     ///< The required surface depth alignment (<c>>=1</c> for volume textures, or 1 for all other surfaces.)
			uint32_t m_bitsPerElement; ///< The number of bits per surface element, as reported by DataFormat::getTotalBitsPerElement().
			struct {
				uint16_t m_isTexCompatible : 1;  ///< <c>true</c> if the surface can be sampled as a texture.
				uint16_t m_unused          : 15; ///< Unused bits
			};
			Gnm::TileMode m_tileMode;  ///< The tile mode that should be used by this surface. Due to complex hardware restrictions, this may be different than the tile mode passed by the user in TilingParameters.
		};
#ifdef _MSC_VER
#pragma warning(pop)
#endif
		
		/**
		 * @brief Based on the specified surface parameters, determines the optimal surface parameters according to various hardware
		 *			restrictions (padding, alignment, myriad tilemode limitations, and so on.).
		 *
		 * This function is only intended for <i>regular</i> surfaces (textures, render targets, etc.).
		 * For CMASK, FMASK, or HTILE surfaces, use computeCmaskInfo(), computeFmaskInfo() or computeHtileInfo(), respectively.
		 *
		 * @param[out] infoOut		The actual surface parameters expected by the hardware will be written to this object. Dimensions are padded,
		 *							actual array modes and tile modes may be different thn those requested, and so on. Use these values instead of the ones in <c><i>infoIn</i></c>.
		 *							This pointer must not be <c>NULL</c>.
		 * @param[in] tp			The input surface parameters. Fill this structure with the requested surface parameters. This pointer must not be <c>NULL</c>.
		 *		 
		 * @return					A status code from GpuAddress::Status.
		 *
		 * @sa    computeCmaskInfo(), computeFmaskInfo(), computeHtileInfo()
		 */
		int32_t computeSurfaceInfo(SurfaceInfo *infoOut, const TilingParameters *tp);

		/**
		 * @brief Based on the specified RenderTarget, determines the optimal CMASK parameters according to various hardware
		 *        restrictions (padding, alignment, and so on.)
		 *
		 * @param[out] outCmaskSizeBytes	If non-<c>NULL</c>, the final padded size (in bytes) of the CMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outCmaskAlign		If non-<c>NULL</c>, the final alignment requirement (in bytes) of the CMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outCmaskPitch		If non-<c>NULL</c>, the final padded pitch (in pixels) of the CMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outCmaskHeight		If non-<c>NULL</c>, the final padded height (in pixels) of the CMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[in] target                The RenderTarget associated with this CMASK buffer. The target's pitch, height, slice count, and more will be used to determine the CMASK buffer properties.
		 *		 
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa    computeSurfaceInfo()
		 */
		int32_t computeCmaskInfo(uint64_t *outCmaskSizeBytes, uint32_t *outCmaskAlign, uint32_t *outCmaskPitch, uint32_t *outCmaskHeight,
			 const Gnm::RenderTarget *target);

		/**
		 * @deprecated This function now takes a Gnm::RenderTarget argument, from which infers its inputs.
		 * @brief Based on the specified CMASK parameters, determines the optimal CMASK parameters according to various hardware
		 *        restrictions (padding, alignment, and so on.)
		 *
		 * @param[out] outCmaskSizeBytes	If non-<c>NULL</c>, the final padded size (in bytes) of the CMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outCmaskAlign		If non-<c>NULL</c>, the final alignment requirement (in bytes) of the CMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outCmaskPitch		If non-<c>NULL</c>, the final padded pitch (in pixels) of the CMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outCmaskHeight		If non-<c>NULL</c>, the final padded height (in pixels) of the CMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[in] rtPitch				The pitch of the corresponding render target, in pixels. This should be the final, possibly padded value
		 *									that computeSurfaceInfo() returns, not the actual unpadded value.
		 * @param[in] rtHeight				The height of the corresponding render target, in pixels. This should be the final, possibly padded value
		 *									that computeSurfaceInfo() returns, not the actual unpadded value.
		 * @param[in] rtNumSlices			The number of slices of the corresponding render target. This should be the final, possibly padded value
		 *									that computeSurfaceInfo() returns, not the actual unpadded value.
		 * @param[in] isCmaskLinear			Specifies whether the CMASK data should be stored linearly or in its own tiled format. It is less efficient to work with linear CMASKs, but they are more compact.
											Because CMASK data is already relatively small, it is usually preferable to specify <c>false</c>.
		 *		 
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa    computeSurfaceInfo()
		 */
		
		int32_t computeCmaskInfo(uint64_t *outCmaskSizeBytes, uint32_t *outCmaskAlign, uint32_t *outCmaskPitch, uint32_t *outCmaskHeight,
			uint32_t rtPitch, uint32_t rtHeight, uint32_t rtNumSlices, bool isCmaskLinear);

		/**
		 * @brief Based on the specified RenderTarget, determines the optimal FMASK parameters according to various hardware
		 *        restrictions (padding, alignment, and so on.)
		 *
		 * @param[out] outFmaskSizeBytes	If non-<c>NULL</c>, the final padded size (in bytes) of the FMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outFmaskAlign		If non-<c>NULL</c>, the final alignment requirement (in bytes) of the FMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outFmaskPitch		If non-<c>NULL</c>, the final padded pitch (in pixels) of the FMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outFmaskHeight		If non-<c>NULL</c>, the final padded height (in pixels) of the FMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[in] target                The RenderTarget associated with this FMASK buffer. The target's pitch, height, slice count, etc. will be used to determine the FMASK buffer properties.
		 *		 
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa    computeSurfaceInfo()
		 */
		int32_t computeFmaskInfo(uint64_t *outFmaskSizeBytes, uint32_t *outFmaskAlign, uint32_t *outFmaskPitch, uint32_t *outFmaskHeight,
			const Gnm::RenderTarget *target);

		/**
		 * @deprecated This function now takes a Gnm::RenderTarget argument, from which infers its inputs.
		 * @brief Based on the specified FMASK parameters, determines the optimal FMASK parameters according to various hardware
		 *        restrictions (padding, alignment, and so on.)
		 *
		 * @param[out] outFmaskSizeBytes	If non-<c>NULL</c>, the final padded size (in bytes) of the FMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outFmaskAlign		If non-<c>NULL</c>, the final alignment requirement (in bytes) of the FMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outFmaskPitch		If non-<c>NULL</c>, the final padded pitch (in pixels) of the FMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outFmaskHeight		If non-<c>NULL</c>, the final padded height (in pixels) of the FMASK surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[in] rtPitch				The pitch of the corresponding render target, in pixels. This should be the final, possibly padded value
		 *									that computeSurfaceInfo() returns, not the actual unpadded value.
		 * @param[in] rtHeight				The height of the corresponding render target, in pixels. This should be the final, possibly padded value
		 *									that computeSurfaceInfo() returns, not the actual unpadded value.
		 * @param[in] rtNumSlices			The number of slices of the corresponding render target. This should be the final, possibly padded value
		 *									that computeSurfaceInfo() returns, not the actual unpadded value.
		 * @param[in] fmaskTileMode			The desired tile mode of the FMASK surface.
		 * @param[in] fmaskFormat			The data format of each FMASK element.
		 *		 
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa    computeSurfaceInfo()
		 */
		
		int32_t computeFmaskInfo(uint64_t *outFmaskSizeBytes, uint32_t *outFmaskAlign, uint32_t *outFmaskPitch, uint32_t *outFmaskHeight,
			uint32_t rtPitch, uint32_t rtHeight, uint32_t rtNumSlices, Gnm::TileMode fmaskTileMode, Gnm::DataFormat fmaskFormat);

		/**
		 * @brief Based on the specified DepthRenderTarget, determines the optimal HTILE parameters according to various hardware
		 *        restrictions (padding, alignment, and so on.)
		 *
		 * @param[out] outHtileSizeBytes	If non-<c>NULL</c>, the final padded size (in bytes) of the HTILE surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outHtileAlign		If non-<c>NULL</c>, the final alignment requirement (in bytes) of the HTILE surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outHtilePitch		If non-<c>NULL</c>, the final padded pitch (in pixels) of the HTILE surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outHtileHeight		If non-<c>NULL</c>, the final padded height (in pixels) of the HTILE surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[in] depthTarget           The DepthRenderTarget associated with this HTILE buffer. The target's pitch, height, slice count, etc. will be used to determine the HTILE buffer properties.
		 *		 
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa    computeSurfaceInfo()
		 */
		int32_t computeHtileInfo(uint64_t *outHtileSizeBytes, uint32_t *outHtileAlign, uint32_t *outHtilePitch, uint32_t *outHtileHeight,
			const Gnm::DepthRenderTarget *depthTarget);

		/**
		 * @deprecated This function now takes a Gnm::DepthRenderTarget argument, from which infers its inputs.
		 * @brief Based on the specified HTILE parameters, determines the optimal HTILE parameters according to various hardware
		 *        restrictions (padding, alignment, and so on.)
		 *
		 * @param[out] outHtileSizeBytes	If non-<c>NULL</c>, the final padded size (in bytes) of the HTILE surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outHtileAlign		If non-<c>NULL</c>, the final alignment requirement (in bytes) of the HTILE surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outHtilePitch		If non-<c>NULL</c>, the final padded pitch (in pixels) of the HTILE surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[out] outHtileHeight		If non-<c>NULL</c>, the final padded height (in pixels) of the HTILE surface will be written here. Specify <c>NULL</c> if not needed.
		 * @param[in] rtPitch				The pitch of the corresponding depth render target, in pixels. Specify the final (possibly padded) value
		 *									that computeSurfaceInfo() returns, not the actual unpadded value.
		 * @param[in] rtHeight				The height of the corresponding depth render target, in pixels. Specify the final (possibly padded) value
		 *									that computeSurfaceInfo() returns, not the actual unpadded value.
		 * @param[in] rtNumSlices			The number of slices of the corresponding depth render target. Specify the final (possibly padded) value
		 *									that computeSurfaceInfo() returns, not the actual unpadded value.
		 * @param[in] isHtileLinear			Specifies whether the HTILE data should be stored linearly or in its own tiled format. It is less efficient to work with linear HTILEs, buy they 
		 *									are more compact. Since HTILE data is already relatively small, it is usually preferable to pass <c>false</c> here.
		 *
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa    computeSurfaceInfo()
		 */
		
		int32_t computeHtileInfo(uint64_t *outHtileSizeBytes, uint32_t *outHtileAlign, uint32_t *outHtilePitch, uint32_t *outHtileHeight,
			uint32_t rtPitch, uint32_t rtHeight, uint32_t rtNumSlices, bool isHtileLinear);

		/**
		 * @brief Computes the minimum size and alignment of a DCC metadata buffer according to details about the corresponding
		 *        color surface.
		 * @param[out] outDccSizeBytes        If non-<c>NULL</c>, the size of the DCC buffer (in bytes) will be stored here.
		 * @param[out] outDccAlign            If non-<c>NULL</c>, the alignment requirement (in bytes) of the DCC buffer will be stored here.
		 * @param[in] target                  The RenderTarget associated with this DCC buffer. The target's pitch, height, slice count and more will be used to determine the DCC buffer properties.
		 * @return                            A status code from GpuAddress::Status.
		 */
		int32_t computeDccInfo(uint64_t *outDccSizeBytes, uint32_t *outDccAlign, const Gnm::RenderTarget *target);

		/**
		* @brief Computes the byte offset for an element (tile) of an HTILE surface.
		*
		* @param[out] outOffset			Receives the byte offset for an element (tile) of an HTILE surface if the operation was successful. This argument must not be <c>NULL</c>.
		* @param[in] depthRenderTarget		The DepthRenderTarget to which the HTILE surface belongs.
		* @param[in] tileX					The X-coordinate of the tile to locate.
		* @param[in] tileY					The Y-coordinate of the tile to locate.
		*
		* @return							A status code from GpuAddress::Status.
		*
		* @sa computeHtileInfo()
		*/
		int32_t computeHtileBufferByteOffset(uint32_t *outOffset, const Gnm::DepthRenderTarget *depthRenderTarget, uint32_t tileX, uint32_t tileY);

		/**
		 * @deprecated This function now takes a Gnm::DepthRenderTarget argument instead of <c>tilesWide</c> and <c>isHtileLinear</c>.
		 * @brief Computes the byte offset for an element (tile) of an HTILE surface.
		 *
		 * @param[out] outOffset			Receives the byte offset for an element (tile) of an HTILE surface if the operation was successful. This argument must not be <c>NULL</c>.
		 * @param[in] tileX					The X-coordinate of the tile to locate.
		 * @param[in] tileY					The Y-coordinate of the tile to locate.
		 * @param[in] tilesWide				The width of the HTILE surface, expressed as a number of tiles.
		 * @param[in] isHtileLinear			Specifies whether to calculate the offset for a linear HTILE surface. The HTILE surface is assumed to be tiled if a value of <c>false</c> is supplied.
		 *		 
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @sa computeHtileInfo()
		 */
		
		int32_t computeHtileBufferByteOffset(uint32_t *outOffset, uint32_t tileX, uint32_t tileY, uint32_t tilesWide, bool isHtileLinear);

		/**
		* @brief Computes the byte offset for an element (tile) of a CMASK surface.
		*
		* @param[out]	outOffset				Receives the nybble offset for an element (tile) of a CMASK surface if the operation was successful. The byte offset is shifted left by one and the LSB indicates the nybble that contains the tile. A value of <c>0</c> indicates the tile in the first nybble, and a value of <c>1</c> indicates the tile in the second nybble. This argument must not be <c>NULL</c>.
		* @param[in]   renderTarget			The RenderTarget to which the CMASK surface belongs.
		* @param[in]	tileX					The X-coordinate of the tile to locate.
		* @param[in]	tileY					The Y-coordinate of the tile to locate.
		*
		* @return							A status code from GpuAddress::Status.
		*/
		int32_t computeCmaskBufferNybbleOffset(uint32_t *outOffset, const Gnm::RenderTarget *renderTarget, uint32_t tileX, uint32_t tileY);

		/** @deprecated This function was renamed to computeCmaskBufferNybbleOffset(), and the new implementation infers the <c><i>tilesWide</i></c> and <c><i>isCmaskLinear</i></c> arguments from an existing RenderTarget object.
		* @brief Computes the byte offset for an element (tile) of a CMASK surface.
		*
		* @param[out]	outOffset				Receives the nybble offset for an element (tile) of a CMASK surface if the operation was successful. The byte offset is shifted left by one and the LSB indicates the nybble that contains the tile. A value of <c>0</c> indicates the tile in the first nybble, and a value of <c>1</c> indicates the tile in the second nybble. This argument must not be <c>NULL</c>.
		* @param[in]	tileX					The X-coordinate of the tile to locate.
		* @param[in]	tileY					The Y-coordinate of the tile to locate.
		* @param[in]	tilesWide				The width (in tiles) of the CMASK surface.
		* @param[in]	isCmaskLinear			A flag that specifies whether to calculate the offset for a linear CMASK surface. The CMASK surface is assumed to be tiled if a value of <c>false</c> is supplied.
		*
		* @return							A status code from GpuAddress::Status.
		*/
		int32_t computeCmaskBufferByteOffset(uint32_t *outOffset, uint32_t tileX, uint32_t tileY, uint32_t tilesWide, bool isCmaskLinear);

#ifndef DOXYGEN_IGNORE
		/**
		 * @brief Determines the micro tile mode for the specified tiling mode.
		 *
		 * The micro tile mode gives a general indication of the category of surface for which the tiling mode 
		 * should be used (for example,  displayable, depth, thick/volume, and so on.)
		 *
		 * @param[out] outMicroTileMode		If successful, the micro tile mode will be stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t getMicroTileMode(Gnm::MicroTileMode *outMicroTileMode, Gnm::TileMode tileMode);

		/**
		 * @brief Determines the array mode for the specified tiling mode.
		 *
		 * Roughly, the array mode determines the overall tiling algorithm for a surface (linear, 1D/microtiled, or 2D/3D/macrotiled.)
		 *
		 * @param[out] outArrayMode			If successful, the array mode will be stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t getArrayMode(Gnm::ArrayMode *outArrayMode, Gnm::TileMode tileMode);

		/**
		 * @brief Determines the pipe configuration for the specified tiling mode.
		 *
		 * This is mainly for internal use by the tiling code.
		 *
		 * @param[out] outPipeConfig		If successful, the pipe configuration will be stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t getPipeConfig(Gnm::PipeConfig *outPipeConfig, Gnm::TileMode tileMode);

		/**
		 * @brief Determines the tile split size for the specified tiling mode.
		 *
		 * This is mainly for internal use by the tiling code.
		 *
		 * @param[out] outTileSplit			If successful, the tile split size will be stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t getTileSplit(Gnm::TileSplit *outTileSplit, Gnm::TileMode tileMode);

		/**
		 * @brief Determines the sample split size for the specified tiling mode.
		 *
		 * @param[out] outSampleSplit		If successful, the sample split size will be stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t getSampleSplit(Gnm::SampleSplit *outSampleSplit, Gnm::TileMode tileMode);

		/**
		 * @brief Determines the bank width for the specified tiling mode.
		 *
		 * @param[out] outBankWidth			If successful, the bank width is stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query. Must be a macro-tiled (2D/3D) mode.
		 * @param[in] bitsPerElement		The number of bits in a single surface element, as specified by DataFormat::getTotalBitsPerElement(). The valid range is <c>[1..128]</c>.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t getBankWidth(Gnm::BankWidth *outBankWidth, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel);

		/**
		 * @brief Returns the bank height for the specified tiling mode.
		 *
		 * @param[out] outBankHeight		If successful, the bank height is stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query. Must be a macro-tiled (2D/3D) mode.
		 * @param[in] bitsPerElement		The number of bits in a single surface element, as specified by DataFormat::getTotalBitsPerElement(). The valid range is <c>[1..128]</c>.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t getBankHeight(Gnm::BankHeight *outBankHeight, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel);

		/**
		 * @brief Returns the macro tile aspect ratio for the specified tiling mode.
		 *
		 * @param[out] outMacroTileAspect	If successful, the macro tile aspect ratio is stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query. Must be a macro-tiled (2D/3D) mode.
		 * @param[in] bitsPerElement		The number of bits in a single surface element, as specified by DataFormat::getTotalBitsPerElement(). The valid range is <c>[1..128]</c>.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel, for multi-sampled render targets. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t getMacroTileAspect(Gnm::MacroTileAspect *outMacroTileAspect, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel);

		/**
		 * @brief Returns the bank count for the specified tiling mode.
		 *
		 * @param[out] outNumBanks			If successful, the bank count is stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query. Must be a macro-tiled (2D/3D) mode.
		 * @param[in] bitsPerElement		The number of bits in a single surface element, as specified by DataFormat::getTotalBitsPerElement(). The valid range is <c>[1..128]</c>.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t getNumBanks(Gnm::NumBanks *outNumBanks, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel);

		/**
		 * @brief Returns the bank width, bank height, macro tile aspect ratio, and bank count for the
		 *        specified tiling mode.
		 *
		 * @param[in] tileMode				The tiling mode to query. Must be a macro-tiled (2D/3D) mode.
		 * @param[in] bitsPerElement		The number of bits in a single surface element, as reported by DataFormat::getTotalBitsPerElement(). The valid range is <c>[1..128]</c>.
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 * @param[out] outBankWidth			If not <c>NULL</c>, the bank width will be written here.
		 * @param[out] outBankHeight		If not <c>NULL</c>, the bank height will be written here.
		 * @param[out] outMacroTileAspect	If not <c>NULL</c>, the macro tile aspect ratio will be written here.
		 * @param[out] outNumBanks			If not <c>NULL</c>, the bank count will be written here.
		 *
		 * @return							A status code from GpuAddress::Status.
		 *
		 * @note It is more efficient to call this function than to retrieve the fields individually.
		 */
		int32_t getAllMacroTileData(Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel,
			Gnm::BankWidth *outBankWidth, Gnm::BankHeight *outBankHeight, Gnm::MacroTileAspect *outMacroTileAspect,
			Gnm::NumBanks *outNumBanks);


		/**
		 * @brief Determines the alternate pipe configuration for the specified tiling mode.
		 *
		 * This function is mainly for internal use by the tiling code.
		 *
		 * @param[out] outAltPipeConfig		If successful, the alternate pipe configuration will be stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query.
		 *
		 * @return							A status code from GpuAddress::Status.
		 * @note                            Alternate tile mode data is mandatory in NEO mode, unless backward-compatible tiling behavior is desired.
		 *                                  It has no effect in base mode.
		 */
		int32_t getAltPipeConfig(Gnm::PipeConfig *outAltPipeConfig, Gnm::TileMode tileMode);

		/**
		 * @brief Returns the alternate bank height for the specified tiling mode.
		 *
		 * @param[out] outAltBankHeight		If successful, the alternate bank height is stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query. Must be a macro-tiled (2D/3D) mode.
		 * @param[in] bitsPerElement		The number of bits in a single surface element. For block-compressed surfaces, specify the size of a 4x4 pixel block; otherwise, specify the size of one fragment. The valid range is [1..128].
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return							A status code from GpuAddress::Status.
		 * @note                            Alternate tile mode data is mandatory in NEO mode, unless backward-compatible tiling behavior is desired.
		 *                                  It has no effect in base mode.
		 */
		int32_t getAltBankHeight(Gnm::BankHeight *outAltBankHeight, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel);

		/**
		 * @brief Returns the alternate macro tile aspect ratio for the specified tiling mode.
		 *
		 * @param[out] outAltMacroTileAspect If successful, the alternate macro tile aspect ratio is stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				 The tiling mode to query. Must be a macro-tiled (2D/3D) mode.
		 * @param[in] bitsPerElement		 The number of bits in a single surface element. For block-compressed surfaces, specify the size of a 4x4 pixel block; otherwise, specify the size of one fragment. The valid range is [1..128].
		 * @param[in] numFragmentsPerPixel	 The number of fragments per pixel. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return							 A status code from GpuAddress::Status.
		 * @note                             Alternate tile mode data is mandatory in NEO mode, unless backward-compatible tiling behavior is desired.
		 *                                   It has no effect in base mode.
		 */
		int32_t getAltMacroTileAspect(Gnm::MacroTileAspect *outAltMacroTileAspect, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel);

		/**
		 * @brief Returns the alternate bank count for the specified tiling mode.
		 *
		 * @param[out] outAltNumBanks		If successful, the alternate bank count is stored here. This argument must not be <c>NULL</c>.
		 * @param[in] tileMode				The tiling mode to query. Must be a macro-tiled (2D/3D) mode.
		 * @param[in] bitsPerElement		The number of bits in a single surface element. For block-compressed surfaces, specify the size of a 4x4 pixel block; otherwise specify the size of one fragment. The valid range is [1..128].
		 * @param[in] numFragmentsPerPixel	The number of fragments per pixel. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 *
		 * @return							A status code from GpuAddress::Status.
		 * @note                            Alternate tile mode data is mandatory in NEO mode, unless backward-compatible tiling behavior is desired.
		 *                                  It has no effect in base mode.
		 */
		int32_t getAltNumBanks(Gnm::NumBanks *outAltNumBanks, Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel);

		/**
		 * @brief Returns the bank width, bank height, macro tile aspect ratio, and bank count for the
		 *        specified tiling mode, using the alternate tiling mode fields where relevant.
		 *
		 * @param[in] tileMode				 The tiling mode to query. Must be a macro-tiled (2D/3D) mode.
		 * @param[in] bitsPerElement		 The number of bits in a single surface element. For block-compressed surfaces, give the size of a 4x4 pixel block; otherwise, specify the size of one fragment. The valid range is [1..128].
		 * @param[in] numFragmentsPerPixel	 The number of fragments per pixel. The only valid values are <c>1</c>, <c>2</c>, <c>4</c> or <c>8</c>.
		 * @param[out] outBankWidth			 If not <c>NULL</c>, the bank width will be written here.
		 * @param[out] outAltBankHeight		 If not <c>NULL</c>, the bank height will be written here.
		 * @param[out] outAltMacroTileAspect If not <c>NULL</c>, the macro tile aspect ratio will be written here.
		 * @param[out] outAltNumBanks		 If not <c>NULL</c>, the bank count will be written here.
		 *
		 * @return							 A status code from GpuAddress::Status.
		 *
		 * @note                             It is more efficient to call this function than to retrieve the fields individually.
		 * @note                             Alternate tile mode data is mandatory in NEO mode, unless backward-compatible tiling behavior is desired.
		 *                                   It has no effect in base mode.
		 */
		 int32_t getAllAltMacroTileData(Gnm::TileMode tileMode, uint32_t bitsPerElement, uint32_t numFragmentsPerPixel,
			Gnm::BankWidth *outBankWidth, Gnm::BankHeight *outAltBankHeight, Gnm::MacroTileAspect *outAltMacroTileAspect,
			Gnm::NumBanks *outAltNumBanks);

		/**
		 * @brief Given a TileMode and an ArrayMode, this function returns a new TileMode that most closely matches the original but is guaranteed to
		 *        use the specified ArrayMode.
		 *
		 * @param[in] gpuMode               The GPU mode to assume for this query.
		 * @param[out] outTileMode          If successful, receives a tile mode that is as similar as possible to <c><i>oldTileMode</i></c> but which uses <c><i>newArrayMode</i></c>.
		 * @param[in]  oldTileMode          The previous tile mode.
		 * @param[in]  newArrayMode         The required array mode.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		int32_t adjustTileMode(Gnm::GpuMode gpuMode, Gnm::TileMode *outTileMode, Gnm::TileMode oldTileMode, Gnm::ArrayMode newArrayMode);

		/**
		 * @deprecated This function now takes a Gnm::GpuMode argument.
		 * @brief Given a TileMode and an ArrayMode, this function returns a new TileMode that most closely matches the original but is guaranteed to
		 *        use the specified ArrayMode.
		 *
		 * @param[out] outTileMode          If successful, receives a tile mode that is as similar as possible to <c><i>oldTileMode</i></c> but which uses <c><i>newArrayMode</i></c>.
		 * @param[in]  oldTileMode          The previous tile mode.
		 * @param[in]  newArrayMode         The required array mode.
		 *
		 * @return							A status code from GpuAddress::Status.
		 */
		
		int32_t adjustTileMode(Gnm::TileMode *outTileMode, Gnm::TileMode oldTileMode, Gnm::ArrayMode newArrayMode);

		/**
		 * @brief Determines whether a TileMode uses a macro-tiled (also called 2D-tiled) ArrayMode.
		 * @param[in] tileMode The TileMode to query.
		 * @return <c>true</c> if the specified TileMode uses a macro-tiled ArrayMode; otherwise, <c>false</c>.
		 */
		bool isMacroTiled(Gnm::TileMode tileMode);

		/**
		 * @brief Determines whether a TileMode uses a micro-tiled (also called 1D-tiled) ArrayMode.
		 * @param[in] tileMode The TileMode to query.
		 * @return <c>true</c> if the specified TileMode uses a micro-tiled ArrayMode; otherwise, <c>false</c>.
		 */
		bool isMicroTiled(Gnm::TileMode tileMode);

		/**
		 * @brief Helper class to tile/detile a surface.
		 *
		 * In general it should not be necessary to use this object directly;
		 * instead, use the tileSurface(), detileSurface() and computeElementByteOffset() functions.
		 *
		 * @sa tileSurface(), detileSurface(), computeElementByteOffset()
		 */
		class Tiler
		{
		protected:
			Gnm::GpuMode m_minGpuMode;
			Gnm::TileMode m_tileMode;
			Gnm::ArrayMode m_arrayMode;
			uint32_t m_linearWidth; // in elements
			uint32_t m_linearHeight; // in elements
			uint32_t m_linearDepth; // in pixels
			uint32_t m_paddedWidth; // = pitch, in elements
			uint32_t m_paddedHeight; // in elements
			uint32_t m_paddedDepth; // in pixels
			uint32_t m_bitsPerElement;
			uint64_t m_linearSizeBytes;
			uint64_t m_tiledSizeBytes;
		};

		/**
		 * @brief Helper class to tile/detile a surface with a linear tile mode.
		 *
		 * In general it should not be necessary to use this object directly;
		 * instead, use the tileSurface(), detileSurface() and computeElementByteOffset() functions.
		 *
		 * @sa tileSurface(), detileSurface(), computeElementByteOffset()
		 */
		class TilerLinear : public Tiler
		{
		public:
			/** @brief The default constructor for the TilerLinear class.
			
				The object won't work until init() has been called.
			*/
			TilerLinear();
			
			/**
			 * @brief Constructs and initializes a TilerLinear object.
			 *
			 * @param[in] tp				The tiling parameters describing the surface to tile. This argument must not be <c>NULL</c>.
			 */
			explicit TilerLinear(const TilingParameters *tp);

			/** @brief The destructor for the TilerLinear class.
			*/
			~TilerLinear();

			/** @brief Initializes a TilerLinear object.
			 *
			 * @param[in] tp				The tiling parameters describing the surface to tile.
			 *
			 * @return						A status code from GpuAddress::Status.
			 */
			int32_t init(const TilingParameters *tp);

			/**
			 * @brief Computes the tiled and untiled byte offsets for the specified element in a surface.
			 *
			 * @param[out] outUntiledByteOffset		Receives the byte offset of the specified element in the untiled surface data buffer. This argument must not be <c>NULL</c>.
			 * @param[out] outTiledByteOffset		Receives the byte offset of the specified element in the tiled surface data buffer. This argument must not be <c>NULL</c>.
			 * @param[in] x							The X coordinate of the element to locate. For surface formats with multiple texels per element, specify in units of elements, rather than individual texels.
			 * @param[in] y							The Y coordinate of the element to locate. For surface formats with multiple texels per element, specify in units of elements, rather than individual texels.
			 * @param[in] z							The Z coordinate of the element to locate. For non-volume surfaces, pass <c>0</c>.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t getElementByteOffset(uint64_t *outUntiledByteOffset, uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z) const
			{
				int32_t status = getTiledElementByteOffset(outTiledByteOffset, x, y, z);
				if (status != GpuAddress::kStatusSuccess)
					return status;
				// NOTE: always assumes the linear data has the same dimensions as the tiled data, for legacy reasons.
				// Call computeLinearElementByteOffset() directly for more freedom with the linear dimensions.
				return computeLinearElementByteOffset(outUntiledByteOffset, x, y, z, 0, m_linearWidth, m_linearWidth*m_linearHeight, m_bitsPerElement, 1);
			}

			/**
			 * @brief Computes the tiled byte offset for the specified element in a surface.
			 *
			 * @param[out] outTiledByteOffset		Receives the byte offset of the specified element in a tiled surface data buffer. This argument must not be <c>NULL</c>.
			 * @param[in] x							The X coordinate of the element to locate. For surface formats with multiple texels per element, specify in units of elements, rather than individual texels.
			 * @param[in] y							The Y coordinate of the element to locate. For surface formats with multiple texels per element, specify in units of elements, rather than individual texels.
			 * @param[in] z							The Z coordinate of the element to locate. For non-volume surfaces, pass 0.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t getTiledElementByteOffset(uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z) const;

			/**
			 * @brief Computes the tiled bit offset for the specified element in a surface.
			 *
			 * @param[out] outTiledBitOffset		Receives the bit offset of the specified element in a tiled surface data buffer. This argument must not be <c>NULL</c>.
			 * @param[in] x							The X coordinate of the element to locate. For surface formats with multiple texels per element, specify in units of elements, rather than individual texels.
			 * @param[in] y							The Y coordinate of the element to locate. For surface formats with multiple texels per element, specify in units of elements, rather than individual texels.
			 * @param[in] z							The Z coordinate of the element to locate. For non-volume surfaces, pass 0.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t getTiledElementBitOffset(uint64_t *outTiledBitOffset, uint32_t x, uint32_t y, uint32_t z) const;

			/**
			 * @brief Tiles an entire surface at once, using the previously specified tiling parameters.
			 *
			 * @param[out] outTiledSurface			The tiled surface data will be written here. Use computeTiledSurfaceSize() to determine the minimum required buffer size. This argument must not be <c>NULL</c>.
			 * @param[in] inUntiledSurface			The source untiled surface data will be read from here. Use computeUntiledSurfaceSize() to determine the amount of data that this function will read from the <c>untiledSurface</c> buffer. This argument must not be <c>NULL</c>.
			 *
			 * @return A status code from GpuAddress::Status.
			 */
			int32_t tileSurface(void *outTiledSurface, const void *inUntiledSurface);

			/**
			 * @brief Tiles a subregion of a surface, using the previously specified tiling parameters.
			 *
			 * @param[out] outTiledPixels   		The base address of the destination surface, which will receive the tiled pixels. Use computeTiledSurfaceSize() to determine the minimum required buffer size. This argument must not be <c>NULL</c>.
			 * @param[in] inUntiledPixels			The source untiled pixels will be read from here. The size of this buffer must be at least as large as
			 *										<c><i>(destRegion->m_back - destRegion->m_front)*sourceSliceSizeElems*(tp->m_element.getBitsPerElement()/8)</i></c>. This argument must not be <c>NULL</c>.
			 * @param[in] destRegion				Describes the bounds of the region in the destination surface where tiled data should be written.
			 *										The region must not extend beyond the bounds of the destination surface.
			 *										If the region is empty (left==right, top==bottom, or front==back), this function will return immediately. This argument must not be <c>NULL</c>.
			 * @param[in] sourcePitchElems			The pitch of the <c>untiledPixels</c> data, expressed as a number of elements (blocks for BCn, pixels otherwise). Must be at least as large as destRegion's width (right-left).
			 * @param[in] sourceSlicePitchElems		The size of one depth slice of the <c>untiledPixels</c> data, expressed as a number of elements (blocks for BCn, pixels otherwise). Must be at least as large as the width of <c><i>destRegion</i></c> (right-left) multiplied by its height (bottom-top).
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t tileSurfaceRegion(void *outTiledPixels, const void *inUntiledPixels, const SurfaceRegion *destRegion, uint32_t sourcePitchElems, uint32_t sourceSlicePitchElems);

			/**
			 * @brief Detiles an entire surface at once, using the previously specified tiling parameters.
			 *
			 * @param[out] outUntiledPixels         The untiled pixels will be written here. Use computeUntiledSurfaceSize() to determine the minimum required buffer size. This argument must not be <c>NULL</c>.
			 * @param[in] inTiledPixels             The source tiled pixels will be read from here. Use computeTiledSurfaceSize() to determine the amount of data that this function will read from the <c>tiledPixels</c> buffer. This argument must not be <c>NULL</c>.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t detileSurface(void *outUntiledPixels, const void *inTiledPixels);

			/**
			 * @brief Detiles a subregion of a surface, using the previously specified tiling parameters.
			 * @param[out] outUntiledPixels			The destination buffer. The detiled pixels will be written here. The size of this buffer must be at least as large as
			 *										<c>(<i>srcRegion</i>-><i>m_back</i> - <i>srcRegion</i>-><i>m_front</i>)*<i>destSlicePitch</i>*(tp-><i>m_bitsPerFragment</i>/8)</c>.
			 * @param[in] tiledPixels				Base address of the source tiled pixels. Use computeTiledSurfaceSize() to determine the amount of data that this function will read from the <c>tiledPixels</c> buffer.
			 * @param[in] srcRegion					Describes the bounds of the region in the source surface from which the tiled data should be read.
			 *										The region must not extend beyond the bounds of the destination buffer.
			 *										If the region is empty (left==right, top==bottom, or front==back), this function returns immediately.
			 * @param[in] destPitch					The pitch of the <c>outUntiledPixels</c> data, given in elements (blocks for BCn, pixels otherwise). Must be at least as large as the width (right-left) of <c><i>srcRegion</i></c>.
			 * @param[in] destSlicePitch			The size of one depth slice of the <c>outUntiledPixels</c> data, given in elements (blocks for BCn, pixels otherwise). Must be at least as large as srcRegion's width (right-left) multiplied by its height (bottom-top).
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t detileSurfaceRegion(void *outUntiledPixels, const void *tiledPixels, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitch);
		};

		/**
		 * @brief Helper class to tile/detile a micro-tiled surface (that is, a surface that has a 1D tile mode.)
		 *
		 * In general it should not be necessary to use this object directly;
		 * instead, use the tileSurface(), detileSurface() and computeElementByteOffset() functions.
		 *
		 * @sa tileSurface(), detileSurface(), computeElementByteOffset()
		 */
		class Tiler1d : public Tiler
		{
		public:
			/** @brief The default constructor for the Tiler1d class.
			
				The object won't work until init() has been called.
			
			*/
			Tiler1d();
			
			/**
			 * @brief Constructs and initializes a Tiler1d object.
			 *
			 * @param[in] tp				The tiling parameters describing the surface to tile.
			 */
			explicit Tiler1d(const TilingParameters *tp);

			/** @brief The destructor for the Tiler1d class.*/
			~Tiler1d();

			/**
			 * @brief Initializes a Tiler1d object.
			 *
			 * @param[in] tp				The tiling parameters describing the surface to tile.
			 *
			 * @return						A status code from GpuAddress::Status.
			 */
			int32_t init(const TilingParameters *tp);

			/**
			 * @brief Computes the tiled and untiled byte offsets for the specified element in a surface.
			 *
			 * @param[out] outUntiledByteOffset		Receives the byte offset of the specified element in an untiled pixel buffer. This argument must not be <c>NULL</c>.
			 * @param[out] outTiledByteOffset		Receives the byte offset of the specified element in a tiled pixel buffer. This argument must not be <c>NULL</c>.
			 * @param[in] x							The X coordinate of the element to locate. For block-compressed surfaces, argument must be specified in units of 4x4 pixel blocks rather than as individual pixels.
			 * @param[in] y							The Y coordinate of the element to locate. For block-compressed surfaces, argument must be specified in units of 4x4 pixel blocks rather than as individual pixels.
			 * @param[in] z							The Z coordinate of the element to locate. For non-volume surfaces, pass <c>0</c>.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t getElementByteOffset(uint64_t *outUntiledByteOffset, uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z) const
			{
				int32_t status = getTiledElementByteOffset(outTiledByteOffset, x, y, z);
				if (status != GpuAddress::kStatusSuccess)
					return status;
				// NOTE: always assumes the linear data has the same dimensions as the tiled data, for legacy reasons.
				// Call computeLinearElementByteOffset() directly for more freedom with the linear dimensions.
				return computeLinearElementByteOffset(outUntiledByteOffset, x, y, z, 0, m_linearWidth, m_linearWidth*m_linearHeight, m_bitsPerElement, 1);
			}

			/**
			 * @brief Computes the tiled byte offset for the specified element in a surface.
			 *
			 * @param[out] outTiledByteOffset		Receives the byte offset of the specified element in a tiled pixel buffer. This argument must not be <c>NULL</c>.
			 * @param[in] x							The X coordinate of the pixel containing the element to locate. For block-compressed surfaces, this argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] y							The Y coordinate of the pixel containing the element to locate. For block-compressed surfaces, this argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] z							The Z coordinate of the pixel containing the element to locate. For non-volume surfaces, pass 0.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t getTiledElementByteOffset(uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z) const;

			/**
			 * @brief Computes the tiled bit offset for the specified element in a surface.
			 *
			 * @param[out] outTiledBitOffset		Receives the bit offset of the specified element in a tiled pixel buffer. This argument must not be <c>NULL</c>.
			 * @param[in] x							The X coordinate of the pixel containing the element to locate. For block-compressed surfaces, argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] y							The Y coordinate of the pixel containing the element to locate. For block-compressed surfaces, argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] z							The Z coordinate of the pixel containing the element to locate. For non-volume surfaces, pass 0.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t getTiledElementBitOffset(uint64_t *outTiledBitOffset, uint32_t x, uint32_t y, uint32_t z) const;

			/**
			 * @brief Tiles an entire surface at once, using the previously specified tiling parameters.
			 *
			 * @param[out] outTiledPixels			The tiled pixels will be written here. Use computeTiledSurfaceSize() to determine the minimum required buffer size.
			 * @param[in] untiledPixels				Buffer from which this function reads source untiled pixels. Use computeUntiledSurfaceSize() to determine the amount of data that this function will read from the <c>untiledPixels</c> buffer.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t tileSurface(void *outTiledPixels, const void *untiledPixels);

			/**
			 * @brief Tiles a subregion of a surface, using the previously specified tiling parameters.
			 *
			 * @param[out] outTiledPixels   		Base address of the destination surface. The tiled pixels will be written here. Use computeTiledSurfaceSize() to determine the minimum required buffer size.
			 * @param[in] inUntiledPixels			Buffer from which this function reads source untiled pixels. The size of this buffer must be at least as large as
			 *										<c><i>(destRegion->m_back - destRegion->m_front)*sourceSlicePitch*(tp->bitsPerFragment/8)</i></c>.
			 * @param[in] destRegion				Describes the bounds of the region in the destination surface to which tiled data should be written.
			 *										The region must not extend beyond the bounds of the destination surface.
			 *										If the region is empty (left==right, top==bottom, or front==back), this function returns immediately.
			 * @param[in] sourcePitch				The pitch of the <c>untiledPixels</c> data, expressed as a number of elements (blocks for BCn, pixels otherwise). Must be at least as large as the width (right-left) of <c><i>destRegion</i></c>.
			 * @param[in] sourceSlicePitch			The size of one depth slice of the <c>untiledPixels</c> data, specified as a number of elements (blocks for BCn, pixels otherwise). Must be at least as large as the width (right-left) of <c><i>destRegion</i></c> multiplied by its height (bottom-top).
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t tileSurfaceRegion(void *outTiledPixels, const void *inUntiledPixels, const SurfaceRegion *destRegion, uint32_t sourcePitch, uint32_t sourceSlicePitch);

			/**
			 * @brief Detiles an entire surface at once, using the previously specified tiling parameters.
			 *
			 * @param[out] outUntiledPixels			The untiled pixels will be written here. Use computeUntiledSurfaceSize() to determine the minimum required buffer size.
			 * @param[in] tiledPixels				Buffer from which this method reads source tiled pixels. Use computeTiledSurfaceSize() to determine the amount of data that this function will read from the <c>tiledPixels</c> buffer.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t detileSurface(void *outUntiledPixels, const void *tiledPixels);

			/**
			 * @brief Detiles a subregion of a surface, using the previously specified tiling parameters.
			 *
			 * @param[out] outUntiledPixels			The destination buffer. The detiled pixels will be written here. The size of this buffer must be at least as large as
			 *										<c><i>(srcRegion->m_back - srcRegion->m_front)*destSlicePitch*(tp->m_bitsPerFragment/8)</i></c>.
			 * @param[in] tiledPixels				Base address of the source tiled pixels. Use computeTiledSurfaceSize() to determine the amount of data that this function will read from the <c>tiledPixels</c> buffer.
			 * @param[in] srcRegion					Describes the bounds of the region in the source surface from which the tiled data is to be read.
			 *										The region must not extend beyond the bounds of the destination buffer.
			 *										If the region is empty (left==right, top==bottom, or front==back), this function returns immediately.
			 * @param[in] destPitch					The pitch of the <c>outUntiledPixels</c> data, specified in elements (blocks for BCn, pixels otherwise). Must be at least as large as the width (right-left) of <c><i>srcRegion</i></c>.
			 * @param[in] destSlicePitch			The size of one depth slice of the <c>outUntiledPixels</c> data, specified in elements (blocks for BCn, pixels otherwise). Must be at least as large as the width (right-left) of <c><i>srcRegion</i></c> multiplied by its height (bottom-top).
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t detileSurfaceRegion(void *outUntiledPixels, const void *tiledPixels, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitch);

		private:
			Gnm::MicroTileMode m_microTileMode;
			// constants
			uint32_t m_tileThickness;
			uint32_t m_tileBytes;
			uint32_t m_tilesPerRow;
			uint32_t m_tilesPerSlice;
		};

		/**
		 * @brief Helper class to tile/detile a macro-tiled surface (that is, a surface that has a 2D/3D tile mode).
		 *
		 * In general it should not be necessary to use this object directly;
		 * instead, use the tileSurface(), detileSurface() and computeElementByteOffset() functions.
		 *
		 * @sa tileSurface(), detileSurface(), computeElementByteOffset()
		 */
		class Tiler2d : public Tiler
		{
		public:
			/** @brief The default constructor for the Tiler2D class.
			*
			*	The object won't work until init() has been called.
			*/
			Tiler2d();
			
			/** @brief Constructs and initializes a Tiler2D object.
			 *
			 * @param[in] tp				The tiling parameters describing the surface to tile.
			 */
			explicit Tiler2d(const TilingParameters *tp);

			/** @brief The destructor for the Tiler2D class.
			*/
			~Tiler2d();

			/** @brief Initializes a Tiler2D object.
			 *
			 * @param[in] tp				The tiling parameters describing the surface to tile.
			 *
			 * @return						A status code from GpuAddress::Status.
			 */
			int32_t init(const TilingParameters *tp);


			/**
			 * @brief Computes the tiled and untiled byte offsets for the specified element in a surface.
			 *
			 * @param[out] outUntiledByteOffset		Receives the byte offset of the specified element in an untiled pixel buffer. This argument must not be <c>NULL</c>.
			 * @param[out] outTiledByteOffset		Receives the byte offset of the specified element in a tiled pixel buffer. This argument must not be <c>NULL</c>.
			 * @param[in] x							The X coordinate of the pixel containing the element to locate. For block-compressed surfaces, this argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] y							The Y coordinate of the pixel containing the element to locate. For block-compressed surfaces, this argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] z							The Z coordinate of the pixel containing the element to locate. For non-volume surfaces, pass <c>0</c>.
			 * @param[in] fragmentIndex				The index of the sub-pixel fragment to locate. For non-MSAA surfaces, pass <c>0</c>. The valid range is <c>[0..<i>m_numFragmentsPerPixel</i>-1]</c>.
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t getElementByteOffset(uint64_t *outUntiledByteOffset, uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex) const
			{
				int32_t status = getTiledElementByteOffset(outTiledByteOffset, x, y, z, fragmentIndex);
				if (status != kStatusSuccess)
					return status;
				// NOTE: always assumes the linear data has the same dimensions as the tiled data, for legacy reasons.
				// Call computeLinearElementByteOffset() directly for more freedom with the linear dimensions.
				return computeLinearElementByteOffset(outUntiledByteOffset, x, y, z, fragmentIndex, m_linearWidth, m_linearWidth*m_linearHeight, m_bitsPerElement, m_numFragmentsPerPixel);
			}

			/**
			 * @brief Computes the tiled byte offset for the specified element in a surface.
			 *
			 * @param[out] outTiledByteOffset		Receives the byte offset of the specified element in a tiled pixel buffer. This argument must not be <c>NULL</c>.
			 * @param[in] x							The X coordinate of the pixel containing the element to locate. For block-compressed surfaces, this argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] y							The Y coordinate of the pixel containing the element to locate. For block-compressed surfaces, this argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] z							The Z coordinate of the pixel containing the element to locate. For non-volume surfaces, pass <c>0</c>.
			 * @param[in] fragmentIndex				The index of the sub-pixel element to locate. For non-MSAA surfaces, pass <c>0</c>. The valid range is <c>[0..<i>m_numFragmentsPerPixel</i>-1]</c>.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t getTiledElementByteOffset(uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex) const;

			/**
			 * @brief Computes the tiled bit offset for the specified element in a surface.
			 *
			 * @param[out] outTiledByteOffset		Receives the bit offset of the specified element in a tiled pixel buffer. This argument must not be <c>NULL</c>.
			 * @param[in] x							The X coordinate of the pixel containing the element to locate. For block-compressed surfaces, this argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] y							The Y coordinate of the pixel containing the element to locate. For block-compressed surfaces, this argument must be specified in units of 4x4 pixel blocks, rather than as individual pixels.
			 * @param[in] z							The Z coordinate of the pixel containing the element to locate. For non-volume surfaces, pass <c>0</c>.
			 * @param[in] fragmentIndex				The index of the sub-pixel element to locate. For non-MSAA surfaces, pass <c>0</c>.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t getTiledElementBitOffset(uint64_t *outTiledByteOffset, uint32_t x, uint32_t y, uint32_t z, uint32_t fragmentIndex) const;

			/**
			 * @brief Tiles an entire surface at once, using the previously specified tiling parameters.
			 *
			 * @param[out] outTiledPixels			The tiled pixels will be written here. Use computeTiledSurfaceSize() to determine the minimum required buffer size.
			 * @param[in] untiledPixels				The source untiled pixels will be read from here. Use computeUntiledSurfaceSize() to determine the amount of data that this function will read from the <c>untiledPixels</c> buffer.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t tileSurface(void *outTiledPixels, const void *untiledPixels);

			/**
			 * @brief Tiles a subregion of a surface, using the previously specified tiling parameters.
			 *
			 * @param[out] outTiledPixels   		Base address of the destination surface. The tiled pixels will be written here. Use computeTiledSurfaceSize() to determine the minimum required buffer size.
			 * @param[in] inUntiledPixels			Buffer from which this method reads source untiled pixels. The size of this buffer must be at least as large as
			 *										(destRegion->m_back - destRegion->m_front)*sourceSlicePitch*(tp->m_bitsPerFragment/8).
			 * @param[in] destRegion				Describes the bounds of the region in the destination surface where tiled data should be written.
			 *										The region must not extend beyond the bounds of the destination surface.
			 *										If the region is empty (left==right, top==bottom, or front==back), the function will return immediately.
			 * @param[in] sourcePitch				The pitch of the <c>untiledPixels</c> data, expressed as a number of elements (blocks for BCn, pixels otherwise). Must be at least as large as destRegion's width (right-left).
			 * @param[in] sourceSlicePitch			The size of one depth slice of the <c>untiledPixels</c> data, expressed as a number of elements (blocks for BCn, pixels otherwise). Must be at least as large as the width of <c><i>destRegion</i></c> (right-left) multiplied by its height (bottom-top).
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t tileSurfaceRegion(void *outTiledPixels, const void *inUntiledPixels, const SurfaceRegion *destRegion, uint32_t sourcePitch, uint32_t sourceSlicePitch);

			/**
			 * @brief Tiles one fragment of a subregion of a surface, using the previously specified tiling parameters.
			 *
			 * @param[out] outTiledPixels		    Base address of the destination surface. The tiled pixels will be written here. Use computeTiledSurfaceSize() to determine the minimum required buffer size.
			 * @param[in] inUntiledPixels			Buffer from which this method reads source untiled pixels. The size of this buffer must be at least as large as
			 *										(destRegion->m_back - destRegion->m_front)*sourceSlicePitch*(tp->m_bitsPerFragment/8).
			 * @param[in] destRegion				Describes the bounds of the region in the destination surface to which tiled data shall be written.
			 *										The region must not extend beyond the bounds of the destination surface.
			 *										If the region is empty (left==right, top==bottom, or front==back), this function returns immediately.
			 * @param[in] sourcePitch				The pitch of the <c>untiledPixels</c> data, expressed as a number of elements (blocks for BCn, pixels otherwise). Must be at least as large as the width (right-left) of <c><i>destRegion</i></c>.
			 * @param[in] sourceSlicePitch			The size of one depth slice of the <c>untiledPixels</c> data, expressed as a number of elements (blocks for BCn, pixels otherwise). Must be at least as large as the width (right-left) of <c><i>destRegion</i></c> multiplied by its height (bottom-top).
			 * @param[in] fragment          		Which fragment to tile. 
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t tileSurfaceRegionOneFragment(void *outTiledPixels, const void *inUntiledPixels, const SurfaceRegion *destRegion, uint32_t sourcePitch, uint32_t sourceSlicePitch, uint32_t fragment);

			/**
			 * @brief Detiles an entire surface at once, using the previously specified tiling parameters.
			 *
			 * @param[out] outUntiledPixels			The untiled pixels will be written here. Use computeUntiledSurfaceSize() to determine the minimum required buffer size.
			 * @param[in] tiledPixels				Buffer from which this method reads source tiled pixels. Use computeTiledSurfaceSize() to determine the amount of data that this function will read from the <c>tiledPixels</c> buffer.
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t detileSurface(void *outUntiledPixels, const void *tiledPixels);

			/**
			 * @brief Detiles a subregion of a surface, using the previously specified tiling parameters.
			 *
			 * @param[out] outUntiledPixels			The destination buffer. The detiled pixels will be written here. The size of this buffer must be at least as large as
			 *										<c>(<i>srcRegion</i>-><i>m_back</i> - <i>srcRegion</i>-><i>m_front</i>)*<i>destSlicePitch</i>*(tp-><i>m_bitsPerFragment</i>/8)</c>.
			 * @param[in] inTiledPixels				Base address of the buffer from which this method reads source tiled pixels. Use computeTiledSurfaceSize() to determine the amount of data that this function will read from the <c>tiledPixels</c> buffer..
			 * @param[in] srcRegion					Describes the bounds of the region in the source surface from which the tiled data should be read.
			 *										The region must not extend beyond the bounds of the destination buffer.
			 *										If the region is empty (left==right, top==bottom, or front==back), the function will return immediately.
			 * @param[in] destPitch					The pitch of the outUntiledPixels data, given in elements (blocks for BCn, pixels otherwise). Must be at least as large as the width (right-left) of <c><i>srcRegion</i></c>.
			 * @param[in] destSlicePitch			The size of one depth slice of the <c><i>outUntiledPixels</i></c> data, specified in elements (blocks for BCn, pixels otherwise.) Must be at least as large as the width (right-left) of <c><i>srcRegion</i></c> multiplied by its height (bottom-top).
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
			int32_t detileSurfaceRegion(void *outUntiledPixels, const void *inTiledPixels, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitch);

			/**
			 * @brief Detiles one fragment of a subregion of a surface, using the previously specified tiling parameters.
			 *
			 * @param[out] outUntiledPixels			The destination buffer. The detiled pixels will be written here. The size of this buffer must be at least as large as
			 *										(srcRegion->m_back - srcRegion->m_front)*destSlicePitch*(tp->m_bitsPerFragment/8).
			 * @param[in] inTiledPixels				Base address of the source tiled pixels. Use computeTiledSurfaceSize() to determine the minimum required buffer size.
			 * @param[in] srcRegion					Describes the bounds of the region in the source surface from which the tiled data should be read.
			 *										The region must not extend beyond the bounds of the destination buffer.
			 *										If the region is empty (left==right, top==bottom, or front==back), the function will return immediately.
			 * @param[in] destPitch					The pitch of the outUntiledPixels data, given in elements (blocks for BCn, pixels otherwise). Must be at least as large as srcRegion's width (right-left).
			 * @param[in] destSlicePitch			The size of one depth slice of the outUntiledPixels data, given in elements (blocks for BCn, pixels otherwise). Must be at least as large as srcRegion's width (right-left) times its height (bottom-top).
			 * @param[in] fragment          		Which fragment to detile. 
			 *
			 * @return								A status code from GpuAddress::Status.
			 */
            int32_t detileSurfaceRegionOneFragment(void *outUntiledPixels, const void *inTiledPixels, const SurfaceRegion *srcRegion, uint32_t destPitch, uint32_t destSlicePitch, uint32_t fragment);
		private:
			Gnm::MicroTileMode m_microTileMode;
			Gnm::PipeConfig m_pipeConfig;
			uint32_t m_arraySlice;
			// constants
			uint32_t m_numFragmentsPerPixel;
			uint32_t m_bankWidth;
			uint32_t m_bankHeight;
			uint32_t m_numBanks;
			uint32_t m_macroTileAspect;
			uint32_t m_tileSplitBytes;

			uint32_t m_numPipes;
			uint32_t m_tileThickness;
			uint32_t m_macroTileWidth;
			uint32_t m_macroTileHeight;

			uint32_t m_pipeInterleaveBytes;
			uint32_t m_pipeInterleaveBits;
			uint32_t m_pipeInterleaveMask;
			uint32_t m_pipeBits;
			uint32_t m_bankBits;
			uint32_t m_pipeMask;
			uint32_t m_bankSwizzleMask;
			uint32_t m_pipeSwizzleMask;
		};
#endif // !DOXYGEN_IGNORE
	}
}
