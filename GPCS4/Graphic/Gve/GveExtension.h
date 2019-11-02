#pragma once

#include "GveCommon.h"

#include <vector>
#include <map>

namespace gve
{;


/**
 * \brief Vulkan extension mode
 *
 * Defines whether an extension is
 * optional, required, or disabled.
 */
enum class GveExtMode 
{
	Disabled,
	Optional,
	Required,
	Passive,
};


/**
 * \brief Vulkan extension info
 *
 * Stores information for a single extension.
 * The renderer can use this information to
 * find out which extensions are enabled.
 */
class GveExt 
{

public:

	GveExt(const char* pName, GveExtMode mode)
		: m_name(pName), m_mode(mode) { }

	/**
	 * \brief Extension name
	 * \returns Extension name
	 */
	const char* name() const 
	{
		return m_name;
	}

	/**
	 * \brief Extension mode
	 * \returns Extension mode
	 */
	GveExtMode mode() const 
	{
		return m_mode;
	}

	/**
	 * \brief Checks whether the extension is enabled
	 *
	 * If an extension is enabled, the features
	 * provided by the extension can be used.
	 * \returns \c true if the extension is enabled
	 */
	operator bool() const 
	{
		return m_revision != 0;
	}

	/**
	 * \brief Supported revision
	 * \returns Supported revision
	 */
	uint32_t revision() const 
	{
		return m_revision;
	}

	/**
	 * \brief Changes extension mode
	 *
	 * In some cases, it may be useful to change the
	 * default mode dynamically after initialization.
	 */
	void setMode(GveExtMode mode) 
	{
		m_mode = mode;
	}

	/**
	 * \brief Enables the extension
	 */
	void enable(uint32_t revision) 
	{
		m_revision = revision;
	}

private:

	const char* m_name = nullptr;
	GveExtMode m_mode = GveExtMode::Disabled;
	uint32_t m_revision = 0;

};


/**
 * \brief Vulkan name list
 *
 * A simple \c vector wrapper that can
 * be used to build a list of layer and
 * extension names.
 */
class GveNameList 
{

public:

	/**
	 * \brief Adds a name
	 * \param [in] pName The name
	 */
	void add(const char* pName) 
	{
		m_names.push_back(pName);
	}

	/**
	 * \brief Number of names
	 * \returns Name count
	 */
	uint32_t count() const 
	{
		return m_names.size();
	}

	/**
	 * \brief Name list
	 * \returns Name list
	 */
	const char* const* names() const 
	{
		return m_names.data();
	}

	/**
	 * \brief Retrieves a single name
	 *
	 * \param [in] index Name index
	 * \returns The given name
	 */
	const char* name(uint32_t index) const 
	{
		return m_names.at(index);
	}

private:

	std::vector<const char*> m_names;

};


/**
 * \brief Vulkan extension set
 *
 * Stores a set of extensions or layers
 * supported by the Vulkan implementation.
 */
class GveNameSet
{

public:

	GveNameSet();
	~GveNameSet();

	/**
	 * \brief Adds a name to the set
	 * \param [in] pName Extension name
	 */
	void add(
		const char*            pName);

	/**
	 * \brief Merges two name sets
	 *
	 * Adds all names from the given name set to
	 * this name set, avoiding duplicate entries.
	 * \param [in] names Name set to merge
	 */
	void merge(
		const GveNameSet&      names);

	/**
	 * \brief Checks whether an extension is supported
	 *
	 * \param [in] pName Extension name
	 * \returns Supported revision, or zero
	 */
	uint32_t supports(
		const char*             pName) const;

	/**
	 * \brief Enables requested extensions
	 *
	 * Walks over a set of extensions and enables all
	 * extensions that are supported and not disabled.
	 * This also checks whether all required extensions
	 * could be enabled, and returns \c false otherwise.
	 * \param [in] numExtensions Number of extensions
	 * \param [in] ppExtensions List of extensions
	 * \param [out] nameSet Extension name set
	 * \returns \c true on success
	 */
	bool enableExtensions(
		uint32_t          numExtensions,
		GveExt**         ppExtensions,
		GveNameSet&      nameSet) const;

	/**
	 * \brief Creates name list from name set
	 *
	 * Adds all names contained in the name set
	 * to a name list, which can then be passed
	 * to Vulkan functions.
	 * \returns Name list
	 */
	GveNameList toNameList() const;

	/**
	 * \brief Enumerates instance layers
	 *
	 * \returns Set of available instance layers
	 */
	static GveNameSet enumInstanceLayers();

	/**
	 * \brief Enumerates instance extensions
	 *
	 * \returns Set of available instance extensions
	 */
	static GveNameSet enumInstanceExtensions();

	/**
	 * \brief Enumerates device extensions
	 *
	 * \param [in] device The device to query
	 * \returns Set of available device extensions
	 */
	static GveNameSet enumDeviceExtensions(VkPhysicalDevice device);

private:

	std::map<std::string, uint32_t> m_names;

};


/**
 * \brief Device extensions
 *
 * Lists all Vulkan extensions that are potentially
 * used by GPCS4 if supported by the implementation.
 */
struct GveDeviceExtensions
{
	GveExt amdMemoryOverallocationBehaviour =		{ VK_AMD_MEMORY_OVERALLOCATION_BEHAVIOR_EXTENSION_NAME,     GveExtMode::Optional };
	GveExt amdShaderFragmentMask =					{ VK_AMD_SHADER_FRAGMENT_MASK_EXTENSION_NAME,               GveExtMode::Optional };
	GveExt extConditionalRendering =				{ VK_EXT_CONDITIONAL_RENDERING_EXTENSION_NAME,              GveExtMode::Optional };
	GveExt extDepthClipEnable =						{ VK_EXT_DEPTH_CLIP_ENABLE_EXTENSION_NAME,                  GveExtMode::Optional };
	GveExt extHostQueryReset =						{ VK_EXT_HOST_QUERY_RESET_EXTENSION_NAME,                   GveExtMode::Optional };
	GveExt extMemoryBudget =						{ VK_EXT_MEMORY_BUDGET_EXTENSION_NAME,                      GveExtMode::Passive };
	GveExt extMemoryPriority =						{ VK_EXT_MEMORY_PRIORITY_EXTENSION_NAME,                    GveExtMode::Optional };
	GveExt extShaderDemoteToHelperInvocation =		{ VK_EXT_SHADER_DEMOTE_TO_HELPER_INVOCATION_EXTENSION_NAME, GveExtMode::Optional };
	GveExt extShaderStencilExport =					{ VK_EXT_SHADER_STENCIL_EXPORT_EXTENSION_NAME,              GveExtMode::Optional };
	GveExt extShaderViewportIndexLayer =			{ VK_EXT_SHADER_VIEWPORT_INDEX_LAYER_EXTENSION_NAME,        GveExtMode::Optional };
	GveExt extTransformFeedback =					{ VK_EXT_TRANSFORM_FEEDBACK_EXTENSION_NAME,                 GveExtMode::Optional };
	GveExt extVertexAttributeDivisor =				{ VK_EXT_VERTEX_ATTRIBUTE_DIVISOR_EXTENSION_NAME,           GveExtMode::Optional };
	GveExt khrCreateRenderPass2 =					{ VK_KHR_CREATE_RENDERPASS_2_EXTENSION_NAME,                GveExtMode::Optional };
	GveExt khrDedicatedAllocation =					{ VK_KHR_DEDICATED_ALLOCATION_EXTENSION_NAME,               GveExtMode::Required };
	GveExt khrDepthStencilResolve =					{ VK_KHR_DEPTH_STENCIL_RESOLVE_EXTENSION_NAME,              GveExtMode::Optional };
	GveExt khrDescriptorUpdateTemplate =			{ VK_KHR_DESCRIPTOR_UPDATE_TEMPLATE_EXTENSION_NAME,         GveExtMode::Required };
	GveExt khrDrawIndirectCount =					{ VK_KHR_DRAW_INDIRECT_COUNT_EXTENSION_NAME,                GveExtMode::Optional };
	GveExt khrDriverProperties =					{ VK_KHR_DRIVER_PROPERTIES_EXTENSION_NAME,                  GveExtMode::Optional };
	GveExt khrGetMemoryRequirements2 =				{ VK_KHR_GET_MEMORY_REQUIREMENTS_2_EXTENSION_NAME,          GveExtMode::Required };
	GveExt khrImageFormatList =						{ VK_KHR_IMAGE_FORMAT_LIST_EXTENSION_NAME,                  GveExtMode::Required };
	GveExt khrMaintenance1 =						{ VK_KHR_MAINTENANCE1_EXTENSION_NAME,                       GveExtMode::Required };
	GveExt khrMaintenance2 =						{ VK_KHR_MAINTENANCE2_EXTENSION_NAME,                       GveExtMode::Required };
	GveExt khrSamplerMirrorClampToEdge =			{ VK_KHR_SAMPLER_MIRROR_CLAMP_TO_EDGE_EXTENSION_NAME,       GveExtMode::Optional };
	GveExt khrShaderDrawParameters =				{ VK_KHR_SHADER_DRAW_PARAMETERS_EXTENSION_NAME,             GveExtMode::Required };
	GveExt khrSwapchain =							{ VK_KHR_SWAPCHAIN_EXTENSION_NAME,                          GveExtMode::Required };
};

/**
 * \brief Instance extensions
 *
 * Lists all Vulkan extensions that are potentially
 * used by GPCS4 if supported by the implementation.
 */
struct GveInstanceExtensions
{
	GveExt khrGetPhysicalDeviceProperties2 =	{ VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME, GveExtMode::Required };
	GveExt khrSurface =							{ VK_KHR_SURFACE_EXTENSION_NAME,                          GveExtMode::Required };
	//GveExt khrWin32Surface =					{ VK_KHR_WIN32_SURFACE_EXTENSION_NAME,                    GveExtMode::Required };
};


}  // namespace gve