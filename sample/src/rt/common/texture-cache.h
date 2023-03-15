/*****************************************************************************
 * Copyright (C), 2023,  Computing & Graphics Research Institute of OPLUS Mobile Comm Corp., Ltd
 * License: No license is required for Oplus internal usage.
 *          No external usage is allowed.
 *
 * File : texture-cache.h
 *
 * Version: 2.0
 *
 * Date : Feb 2023
 *
 * Author: Computing & Graphics Research Institute
 *
 * ------------------ Revision History: ---------------------
 *  <version>  <date>  <author>  <desc>
 *
 *******************************************************************************/

#pragma once

#include <ph/rt-utils.h>

#include <unordered_map>

/// Manages loading and caching images.
class TextureCache {
public:
    TextureCache(ph::va::VulkanSubmissionProxy * vsp, ph::AssetSystem * assetSystem, const VkFormat defaultShadowMapFormat, uint32_t defaultShadowMapSize);

    ~TextureCache() = default;

    ph::AssetSystem * getAssetSystem() { return _assetSystem; }

    const std::unordered_map<std::string, ph::va::ImageObject> & textures() const { return _textureHandles; }

    /// Loads the image at the given asset path. The result is cached,
    /// so further calls with the same path will return the same TextureHandle.
    /// @param assetPath Path in the asset system we want to load an image from.
    /// @return TextureHandle containing the image at the given path.
    ph::rt::Material::TextureHandle loadFromAsset(const std::string & assetPath, VkImageUsageFlagBits usage = VK_IMAGE_USAGE_SAMPLED_BIT);

    /// Gets asset path for a given texture handle. Slow- should only be used outside of runtime.
    /// Currently used for PBRT3 exporter.
    /// @param textureHandle TextureHandle as stored in MaterialDesc.map[*].
    /// @return Path of the original asset used to load the given textureHandle.
    std::string getAssetPath(const ph::rt::Material::TextureHandle & textureHandle);

    /// Converts the given image proxy to a texture handle. The result is cached
    /// until the texture cache is destroyed
    /// @param imageProxy The image proxy to generate a texture handle from.
    /// This will return a new TextureHandle every time you call this, even
    /// if you pass the exact same image proxy multiple times.
    /// @return A TextureHandle wrapping an image representing the given imageProxy.
    /// If imageProxy refers to an empty image, this will return an empty TextureHandle.
    ph::rt::Material::TextureHandle createFromImageProxy(const ph::ImageProxy & imageProxy);

    // Required for properly exporting textures to PBRT3.
    ph::rt::Material::TextureHandle createFromImageProxy(const ph::ImageProxy & imageProxy, std::string imageAssetPath);

    /// Uses a format of VK_FORMAT_R32_SFLOAT and a size of 512.
    /// @param name Name of the the shadow map (for log/debug only)
    /// @return a texture suitable for a 2d shadowmap.
    ph::rt::Material::TextureHandle createShadowMap2D(const char * name);

    /// Uses a format of VK_FORMAT_R32_SFLOAT and a size of 512.
    /// @param name Name of the the shadow map (for log/debug only)
    /// @return a texture suitable for a 3d shadowmap.
    ph::rt::Material::TextureHandle createShadowMapCube(const char * name);

private:
    /// Used to load images into vulkan.
    ph::va::VulkanSubmissionProxy * const _vsp = nullptr;

    /// Used to load selected images.
    ph::AssetSystem * const _assetSystem = nullptr;

    /// Maps texture handle to all relevant info about it.
    std::unordered_map<std::string, ph::va::ImageObject> _textureHandles;

    /// image objects created from ImageProxy. Keeps images stored in Vulkan until texture cache is destroyed.
    std::vector<ph::va::ImageObject> _imageProxyHandles;

    VkFormat _defaultShadowMapFormat = VK_FORMAT_R16_SFLOAT;
    uint32_t _defaultShadowMapSize   = 512;

    /// @param name Name of the the shadow map (for log/debug only)
    /// @param format Image format of the shadow map.
    /// @param size Size of the shadow map.
    /// @return a texture suitable for a 2d shadow map.
    ph::rt::Material::TextureHandle createShadowMap2D(const char * name, VkFormat format, uint32_t size);

    /// @param name Name of the the shadow map (for log/debug only)
    /// @param format Image format of the shadow map.
    /// @param size Size of the shadow map.
    /// @return a texture suitable for a 3d shadow map.
    ph::rt::Material::TextureHandle createShadowMapCube(const char * name, VkFormat format, uint32_t size);
};