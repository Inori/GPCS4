#pragma once

#include "VltCommon.h"

namespace vlt
{;

class VltDevice;
class VltImage;
class VltImageView;
class VltPresenter;

	/**
 * \brief Presenter description
 * 
 * Contains the desired properties of
 * the swap chain. This is passed as
 * an input during swap chain creation.
 */
struct PresenterDesc
{
	VkSurfaceKHR windowSurface;
	VkQueue      presentQueue;
	VkExtent2D   imageExtent;
	uint32_t     imageCount;
	bool         fullScreen;
};

/**
 * \brief Presenter properties
 * 
 * Contains the actual properties
 * of the underlying swap chain.
 */
struct PresenterInfo
{
	VkSurfaceFormatKHR format;
	VkPresentModeKHR   presentMode;
	VkExtent2D         imageExtent;
	uint32_t           imageCount;
};

/**
 * \brief Swap image and view
 */
struct PresenterImage
{
	RcPtr<VltImage>     image;
	RcPtr<VltImageView> view;
};

/**
  * \brief Presenter semaphores
  * 
  * Pair of semaphores used for acquire and present
  * operations, including the command buffers used
  * in between. Also stores a fence to signal on
  * image acquisition.
  */
struct PresenterSync
{
	VkFence     fence;
	VkSemaphore acquire;
	VkSemaphore present;
};


struct SwapChainSupportDetails
{
	VkSurfaceCapabilitiesKHR        capabilities;
	std::vector<VkSurfaceFormatKHR> formats;
	std::vector<VkPresentModeKHR>   presentModes;
};

/**
  * \brief Present information
  * 
  * Used to pass present information to the device.
  */
struct VltPresentInfo
{
	RcPtr<VltPresenter> presenter;
	VkSemaphore         waitSync;
};



class VltPresenter : public RcObject
{
public:
	VltPresenter(const RcPtr<vlt::VltDevice>& device,
				 const PresenterDesc&         desc);
	~VltPresenter();

	/**
     * \brief Actual presenter info
     * \returns Swap chain properties
     */
	PresenterInfo info() const;

	/**
     * \breif Retrieves a pair of semaphores
     * 
     * These semaphores are meant to be used
     * for acquire and present operations.
     * \returns Pair of semaphores
     */
	PresenterSync getSyncObjects() const;

	/**
     * \brief Retrieves image by index
     * 
     * Can be used to create per-image objects.
     * \param [in] index Image index
     * \returns Image handle
     */
	PresenterImage getImage(
		uint32_t index) const;

	/**
     * \brief Acquires next image
     * 
     * Potentially blocks the calling thread.
     * If this returns an error, the swap chain
     * must be recreated and a new image must
     * be acquired before proceeding.
     * \param [in] signal Semaphore to signal
     * \param [in] fence Fence to signal (optional)
     * \param [out] index Acquired image index
     * \returns Status of the operation
     */
	VkResult acquireNextImage(
		VkSemaphore signal,
		VkFence     fence,
		uint32_t&   index);

	/**
     * \brief Waits for fence to get signaled
     *
     * Helper method that can be used in conjunction
     * with the fence passed to \ref acquireNextImage.
     * \param [in] fence Fence to wait on
     * \returns Status of the operation
     */
	VkResult waitForFence(
		VkFence fence);

	/**
     * \brief Presents current image
     * 
     * Presents the current image. If this returns
     * an error, the swap chain must be recreated,
     * but do not present before acquiring an image.
     * \param [in] wait Semaphore to wait on
     * \returns Status of the operation
     */
	VkResult presentImage(
		VkSemaphore wait);

	/**
     * \brief Changes presenter properties
     * 
     * Recreates the swap chain immediately. Note that
     * no swap chain resources must be in use by the
     * GPU at the time this is called.
     * \param [in] desc Swap chain description
     */
	VkResult recreateSwapChain(
		const PresenterDesc& desc);

private:
	SwapChainSupportDetails querySwapChainSupport(
		VkPhysicalDevice     device,
		const PresenterDesc& desc);

	VkSurfaceFormatKHR pickFormat(const SwapChainSupportDetails& details);

	VkPresentModeKHR pickPresentMode(const SwapChainSupportDetails& details);

	VkExtent2D pickImageExtent(
		const SwapChainSupportDetails& details,
		VkExtent2D                     desired);

	uint32_t pickImageCount(
		const SwapChainSupportDetails& details,
		uint32_t                       desired);

	bool initImages();

	bool initSyncObjects();

	void destroySwapchain();

private:
	RcPtr<VltDevice> m_device;

	VkSwapchainKHR              m_swapchain = VK_NULL_HANDLE;
	PresenterInfo               m_info;
	VkQueue                     m_presentQueue;
	std::vector<PresenterImage> m_images;
	std::vector<PresenterSync>  m_syncObjects;

	uint32_t m_frameIndex = 0;
	uint32_t m_imageIndex = 0;
};

}  // namespace vlt