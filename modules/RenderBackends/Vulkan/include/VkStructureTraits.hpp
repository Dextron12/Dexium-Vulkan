
// This header is generated from the library VkReflect <git@github.com:Dextron12/VkReflect.git>
// Maintained and owned by Dextron12 <ethan.mckane.dev@gmail.com>

// <codegen:VersionMarker>
// Version: 0.2.0, 19-09-2026
// <codegen:VersionMarker>

// This header should NOT be modified, instead rerun the Vkreflect tool if you need to modify this file.
// It uses char markers to find code blocks, modifying these markers will invalidate the file.

// --- VULKAN IMPLEMENTATION ---
// This header explicitly includes vulkan/vulkan.h, the C Vulkan API.
// Compile this header with the rest of your project and ensure Vulkan is being linked against your project.

#ifndef CODEGEN_vkReflect_HPP
#define CODEGEN_vkReflect_HPP

#include <vulkan/vulkan.h>

#include <tuple> // heterogeneous container


template<typename Structure, typename Member>
struct VkStructureMember {
    const char* name;
    Member Structure::* ptr;

    using StructureType = Structure;
    using type = Member;

    static constexpr bool isPointer = std::is_pointer_v<Member>;

    static constexpr bool isArray = std::is_array_v<Member>;

    static constexpr size_t arraySize = std::extent_v<Member>;

    constexpr Member& get(Structure& structure) const noexcept {
        return structure.*ptr;
    }

    constexpr const Member& get(const Structure& structure) const noexcept {
        return structure.*ptr;
    }

    template<typename Fn>
    void visit(Structure& structure, Fn&& fn) const {
        std::forward<Fn>(fn)(name, get(structure));
    }

    template<typename Fn>
    void visit(const Structure& structure, Fn&& fn) const {
        std::forward<Fn>(fn)(name, get(structure));
    }
};


// Free-fn forEachMember, allows traversal of an entire structure.
// TODO: https://trello.com/c/DYthJMEE/61-vkreflect-add-foreachmember-fn
// TODO: https://trello.com/c/nr3cOMnp/60-vkreflect-add-visit-fn
// To be clear, both TODO's need to be added to the VkReflect prefabs and generator to record them to the file

// <codegen:baseStructure>
template<typename T>
struct VkStructureTraits;
// <codegen:baseStructure>

// <codegen:beginVkPhysicalDeviceFeatures>

template<typename T>
concept HasVkStructureTraits = requires {
    VkStructureTraits<std::remove_cvref_t<T>>::members;
};

template <>
struct VkStructureTraits<VkPhysicalDeviceFeatures> {
    // <codegen:beginSType>
    // <codegen:endSType>

    static constexpr const char* name = "VkPhysicalDeviceFeatures";

    static constexpr std::tuple members{
    // <codegen:beginMemberArray>
        VkStructureMember{
        "robustBufferAccess",
        &VkPhysicalDeviceFeatures::robustBufferAccess,
    },
    VkStructureMember{
        "fullDrawIndexUint32",
        &VkPhysicalDeviceFeatures::fullDrawIndexUint32,
    },
    VkStructureMember{
        "imageCubeArray",
        &VkPhysicalDeviceFeatures::imageCubeArray,
    },
    VkStructureMember{
        "independentBlend",
        &VkPhysicalDeviceFeatures::independentBlend,
    },
    VkStructureMember{
        "geometryShader",
        &VkPhysicalDeviceFeatures::geometryShader,
    },
    VkStructureMember{
        "tessellationShader",
        &VkPhysicalDeviceFeatures::tessellationShader,
    },
    VkStructureMember{
        "sampleRateShading",
        &VkPhysicalDeviceFeatures::sampleRateShading,
    },
    VkStructureMember{
        "dualSrcBlend",
        &VkPhysicalDeviceFeatures::dualSrcBlend,
    },
    VkStructureMember{
        "logicOp",
        &VkPhysicalDeviceFeatures::logicOp,
    },
    VkStructureMember{
        "multiDrawIndirect",
        &VkPhysicalDeviceFeatures::multiDrawIndirect,
    },
    VkStructureMember{
        "drawIndirectFirstInstance",
        &VkPhysicalDeviceFeatures::drawIndirectFirstInstance,
    },
    VkStructureMember{
        "depthClamp",
        &VkPhysicalDeviceFeatures::depthClamp,
    },
    VkStructureMember{
        "depthBiasClamp",
        &VkPhysicalDeviceFeatures::depthBiasClamp,
    },
    VkStructureMember{
        "fillModeNonSolid",
        &VkPhysicalDeviceFeatures::fillModeNonSolid,
    },
    VkStructureMember{
        "depthBounds",
        &VkPhysicalDeviceFeatures::depthBounds,
    },
    VkStructureMember{
        "wideLines",
        &VkPhysicalDeviceFeatures::wideLines,
    },
    VkStructureMember{
        "largePoints",
        &VkPhysicalDeviceFeatures::largePoints,
    },
    VkStructureMember{
        "alphaToOne",
        &VkPhysicalDeviceFeatures::alphaToOne,
    },
    VkStructureMember{
        "multiViewport",
        &VkPhysicalDeviceFeatures::multiViewport,
    },
    VkStructureMember{
        "samplerAnisotropy",
        &VkPhysicalDeviceFeatures::samplerAnisotropy,
    },
    VkStructureMember{
        "textureCompressionETC2",
        &VkPhysicalDeviceFeatures::textureCompressionETC2,
    },
    VkStructureMember{
        "textureCompressionASTC_LDR",
        &VkPhysicalDeviceFeatures::textureCompressionASTC_LDR,
    },
    VkStructureMember{
        "textureCompressionBC",
        &VkPhysicalDeviceFeatures::textureCompressionBC,
    },
    VkStructureMember{
        "occlusionQueryPrecise",
        &VkPhysicalDeviceFeatures::occlusionQueryPrecise,
    },
    VkStructureMember{
        "pipelineStatisticsQuery",
        &VkPhysicalDeviceFeatures::pipelineStatisticsQuery,
    },
    VkStructureMember{
        "vertexPipelineStoresAndAtomics",
        &VkPhysicalDeviceFeatures::vertexPipelineStoresAndAtomics,
    },
    VkStructureMember{
        "fragmentStoresAndAtomics",
        &VkPhysicalDeviceFeatures::fragmentStoresAndAtomics,
    },
    VkStructureMember{
        "shaderTessellationAndGeometryPointSize",
        &VkPhysicalDeviceFeatures::shaderTessellationAndGeometryPointSize,
    },
    VkStructureMember{
        "shaderImageGatherExtended",
        &VkPhysicalDeviceFeatures::shaderImageGatherExtended,
    },
    VkStructureMember{
        "shaderStorageImageExtendedFormats",
        &VkPhysicalDeviceFeatures::shaderStorageImageExtendedFormats,
    },
    VkStructureMember{
        "shaderStorageImageMultisample",
        &VkPhysicalDeviceFeatures::shaderStorageImageMultisample,
    },
    VkStructureMember{
        "shaderStorageImageReadWithoutFormat",
        &VkPhysicalDeviceFeatures::shaderStorageImageReadWithoutFormat,
    },
    VkStructureMember{
        "shaderStorageImageWriteWithoutFormat",
        &VkPhysicalDeviceFeatures::shaderStorageImageWriteWithoutFormat,
    },
    VkStructureMember{
        "shaderUniformBufferArrayDynamicIndexing",
        &VkPhysicalDeviceFeatures::shaderUniformBufferArrayDynamicIndexing,
    },
    VkStructureMember{
        "shaderSampledImageArrayDynamicIndexing",
        &VkPhysicalDeviceFeatures::shaderSampledImageArrayDynamicIndexing,
    },
    VkStructureMember{
        "shaderStorageBufferArrayDynamicIndexing",
        &VkPhysicalDeviceFeatures::shaderStorageBufferArrayDynamicIndexing,
    },
    VkStructureMember{
        "shaderStorageImageArrayDynamicIndexing",
        &VkPhysicalDeviceFeatures::shaderStorageImageArrayDynamicIndexing,
    },
    VkStructureMember{
        "shaderClipDistance",
        &VkPhysicalDeviceFeatures::shaderClipDistance,
    },
    VkStructureMember{
        "shaderCullDistance",
        &VkPhysicalDeviceFeatures::shaderCullDistance,
    },
    VkStructureMember{
        "shaderFloat64",
        &VkPhysicalDeviceFeatures::shaderFloat64,
    },
    VkStructureMember{
        "shaderInt64",
        &VkPhysicalDeviceFeatures::shaderInt64,
    },
    VkStructureMember{
        "shaderInt16",
        &VkPhysicalDeviceFeatures::shaderInt16,
    },
    VkStructureMember{
        "shaderResourceResidency",
        &VkPhysicalDeviceFeatures::shaderResourceResidency,
    },
    VkStructureMember{
        "shaderResourceMinLod",
        &VkPhysicalDeviceFeatures::shaderResourceMinLod,
    },
    VkStructureMember{
        "sparseBinding",
        &VkPhysicalDeviceFeatures::sparseBinding,
    },
    VkStructureMember{
        "sparseResidencyBuffer",
        &VkPhysicalDeviceFeatures::sparseResidencyBuffer,
    },
    VkStructureMember{
        "sparseResidencyImage2D",
        &VkPhysicalDeviceFeatures::sparseResidencyImage2D,
    },
    VkStructureMember{
        "sparseResidencyImage3D",
        &VkPhysicalDeviceFeatures::sparseResidencyImage3D,
    },
    VkStructureMember{
        "sparseResidency2Samples",
        &VkPhysicalDeviceFeatures::sparseResidency2Samples,
    },
    VkStructureMember{
        "sparseResidency4Samples",
        &VkPhysicalDeviceFeatures::sparseResidency4Samples,
    },
    VkStructureMember{
        "sparseResidency8Samples",
        &VkPhysicalDeviceFeatures::sparseResidency8Samples,
    },
    VkStructureMember{
        "sparseResidency16Samples",
        &VkPhysicalDeviceFeatures::sparseResidency16Samples,
    },
    VkStructureMember{
        "sparseResidencyAliased",
        &VkPhysicalDeviceFeatures::sparseResidencyAliased,
    },
    VkStructureMember{
        "variableMultisampleRate",
        &VkPhysicalDeviceFeatures::variableMultisampleRate,
    },
    VkStructureMember{
        "inheritedQueries",
        &VkPhysicalDeviceFeatures::inheritedQueries,
    }// <codegen:endMemberArray>
    };

};


// <codegen:endVkPhysicalDeviceFeatures>// <codegen:beginVkPhysicalDeviceFeatures2>

template <>
struct VkStructureTraits<VkPhysicalDeviceFeatures2> {
    // <codegen:beginSType>
    static constexpr VkStructureType sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2;

    // <codegen:endSType>

    static constexpr const char* name = "VkPhysicalDeviceFeatures2";

    static constexpr std::tuple members{
    // <codegen:beginMemberArray>
        VkStructureMember{
        "pNext",
        &VkPhysicalDeviceFeatures2::pNext,
    },
    VkStructureMember{
        "features",
        &VkPhysicalDeviceFeatures2::features,
    }// <codegen:endMemberArray>
    };

};


// <codegen:endVkPhysicalDeviceFeatures2>// <codegen:beginVkPhysicalDeviceVulkan11Features>

template <>
struct VkStructureTraits<VkPhysicalDeviceVulkan11Features> {
    // <codegen:beginSType>
    static constexpr VkStructureType sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES;

    // <codegen:endSType>

    static constexpr const char* name = "VkPhysicalDeviceVulkan11Features";

    static constexpr std::tuple members{
    // <codegen:beginMemberArray>
        VkStructureMember{
        "pNext",
        &VkPhysicalDeviceVulkan11Features::pNext,
    },
    VkStructureMember{
        "storageBuffer16BitAccess",
        &VkPhysicalDeviceVulkan11Features::storageBuffer16BitAccess,
    },
    VkStructureMember{
        "uniformAndStorageBuffer16BitAccess",
        &VkPhysicalDeviceVulkan11Features::uniformAndStorageBuffer16BitAccess,
    },
    VkStructureMember{
        "storagePushConstant16",
        &VkPhysicalDeviceVulkan11Features::storagePushConstant16,
    },
    VkStructureMember{
        "storageInputOutput16",
        &VkPhysicalDeviceVulkan11Features::storageInputOutput16,
    },
    VkStructureMember{
        "multiview",
        &VkPhysicalDeviceVulkan11Features::multiview,
    },
    VkStructureMember{
        "multiviewGeometryShader",
        &VkPhysicalDeviceVulkan11Features::multiviewGeometryShader,
    },
    VkStructureMember{
        "multiviewTessellationShader",
        &VkPhysicalDeviceVulkan11Features::multiviewTessellationShader,
    },
    VkStructureMember{
        "variablePointersStorageBuffer",
        &VkPhysicalDeviceVulkan11Features::variablePointersStorageBuffer,
    },
    VkStructureMember{
        "variablePointers",
        &VkPhysicalDeviceVulkan11Features::variablePointers,
    },
    VkStructureMember{
        "protectedMemory",
        &VkPhysicalDeviceVulkan11Features::protectedMemory,
    },
    VkStructureMember{
        "samplerYcbcrConversion",
        &VkPhysicalDeviceVulkan11Features::samplerYcbcrConversion,
    },
    VkStructureMember{
        "shaderDrawParameters",
        &VkPhysicalDeviceVulkan11Features::shaderDrawParameters,
    }// <codegen:endMemberArray>
    };

};


// <codegen:endVkPhysicalDeviceVulkan11Features>// <codegen:beginVkPhysicalDeviceVulkan12Features>

template <>
struct VkStructureTraits<VkPhysicalDeviceVulkan12Features> {
    // <codegen:beginSType>
    static constexpr VkStructureType sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES;

    // <codegen:endSType>

    static constexpr const char* name = "VkPhysicalDeviceVulkan12Features";

    static constexpr std::tuple members{
    // <codegen:beginMemberArray>
        VkStructureMember{
        "pNext",
        &VkPhysicalDeviceVulkan12Features::pNext,
    },
    VkStructureMember{
        "samplerMirrorClampToEdge",
        &VkPhysicalDeviceVulkan12Features::samplerMirrorClampToEdge,
    },
    VkStructureMember{
        "drawIndirectCount",
        &VkPhysicalDeviceVulkan12Features::drawIndirectCount,
    },
    VkStructureMember{
        "storageBuffer8BitAccess",
        &VkPhysicalDeviceVulkan12Features::storageBuffer8BitAccess,
    },
    VkStructureMember{
        "uniformAndStorageBuffer8BitAccess",
        &VkPhysicalDeviceVulkan12Features::uniformAndStorageBuffer8BitAccess,
    },
    VkStructureMember{
        "storagePushConstant8",
        &VkPhysicalDeviceVulkan12Features::storagePushConstant8,
    },
    VkStructureMember{
        "shaderBufferInt64Atomics",
        &VkPhysicalDeviceVulkan12Features::shaderBufferInt64Atomics,
    },
    VkStructureMember{
        "shaderSharedInt64Atomics",
        &VkPhysicalDeviceVulkan12Features::shaderSharedInt64Atomics,
    },
    VkStructureMember{
        "shaderFloat16",
        &VkPhysicalDeviceVulkan12Features::shaderFloat16,
    },
    VkStructureMember{
        "shaderInt8",
        &VkPhysicalDeviceVulkan12Features::shaderInt8,
    },
    VkStructureMember{
        "descriptorIndexing",
        &VkPhysicalDeviceVulkan12Features::descriptorIndexing,
    },
    VkStructureMember{
        "shaderInputAttachmentArrayDynamicIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderInputAttachmentArrayDynamicIndexing,
    },
    VkStructureMember{
        "shaderUniformTexelBufferArrayDynamicIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderUniformTexelBufferArrayDynamicIndexing,
    },
    VkStructureMember{
        "shaderStorageTexelBufferArrayDynamicIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderStorageTexelBufferArrayDynamicIndexing,
    },
    VkStructureMember{
        "shaderUniformBufferArrayNonUniformIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderUniformBufferArrayNonUniformIndexing,
    },
    VkStructureMember{
        "shaderSampledImageArrayNonUniformIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderSampledImageArrayNonUniformIndexing,
    },
    VkStructureMember{
        "shaderStorageBufferArrayNonUniformIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderStorageBufferArrayNonUniformIndexing,
    },
    VkStructureMember{
        "shaderStorageImageArrayNonUniformIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderStorageImageArrayNonUniformIndexing,
    },
    VkStructureMember{
        "shaderInputAttachmentArrayNonUniformIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderInputAttachmentArrayNonUniformIndexing,
    },
    VkStructureMember{
        "shaderUniformTexelBufferArrayNonUniformIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderUniformTexelBufferArrayNonUniformIndexing,
    },
    VkStructureMember{
        "shaderStorageTexelBufferArrayNonUniformIndexing",
        &VkPhysicalDeviceVulkan12Features::shaderStorageTexelBufferArrayNonUniformIndexing,
    },
    VkStructureMember{
        "descriptorBindingUniformBufferUpdateAfterBind",
        &VkPhysicalDeviceVulkan12Features::descriptorBindingUniformBufferUpdateAfterBind,
    },
    VkStructureMember{
        "descriptorBindingSampledImageUpdateAfterBind",
        &VkPhysicalDeviceVulkan12Features::descriptorBindingSampledImageUpdateAfterBind,
    },
    VkStructureMember{
        "descriptorBindingStorageImageUpdateAfterBind",
        &VkPhysicalDeviceVulkan12Features::descriptorBindingStorageImageUpdateAfterBind,
    },
    VkStructureMember{
        "descriptorBindingStorageBufferUpdateAfterBind",
        &VkPhysicalDeviceVulkan12Features::descriptorBindingStorageBufferUpdateAfterBind,
    },
    VkStructureMember{
        "descriptorBindingUniformTexelBufferUpdateAfterBind",
        &VkPhysicalDeviceVulkan12Features::descriptorBindingUniformTexelBufferUpdateAfterBind,
    },
    VkStructureMember{
        "descriptorBindingStorageTexelBufferUpdateAfterBind",
        &VkPhysicalDeviceVulkan12Features::descriptorBindingStorageTexelBufferUpdateAfterBind,
    },
    VkStructureMember{
        "descriptorBindingUpdateUnusedWhilePending",
        &VkPhysicalDeviceVulkan12Features::descriptorBindingUpdateUnusedWhilePending,
    },
    VkStructureMember{
        "descriptorBindingPartiallyBound",
        &VkPhysicalDeviceVulkan12Features::descriptorBindingPartiallyBound,
    },
    VkStructureMember{
        "descriptorBindingVariableDescriptorCount",
        &VkPhysicalDeviceVulkan12Features::descriptorBindingVariableDescriptorCount,
    },
    VkStructureMember{
        "runtimeDescriptorArray",
        &VkPhysicalDeviceVulkan12Features::runtimeDescriptorArray,
    },
    VkStructureMember{
        "samplerFilterMinmax",
        &VkPhysicalDeviceVulkan12Features::samplerFilterMinmax,
    },
    VkStructureMember{
        "scalarBlockLayout",
        &VkPhysicalDeviceVulkan12Features::scalarBlockLayout,
    },
    VkStructureMember{
        "imagelessFramebuffer",
        &VkPhysicalDeviceVulkan12Features::imagelessFramebuffer,
    },
    VkStructureMember{
        "uniformBufferStandardLayout",
        &VkPhysicalDeviceVulkan12Features::uniformBufferStandardLayout,
    },
    VkStructureMember{
        "shaderSubgroupExtendedTypes",
        &VkPhysicalDeviceVulkan12Features::shaderSubgroupExtendedTypes,
    },
    VkStructureMember{
        "separateDepthStencilLayouts",
        &VkPhysicalDeviceVulkan12Features::separateDepthStencilLayouts,
    },
    VkStructureMember{
        "hostQueryReset",
        &VkPhysicalDeviceVulkan12Features::hostQueryReset,
    },
    VkStructureMember{
        "timelineSemaphore",
        &VkPhysicalDeviceVulkan12Features::timelineSemaphore,
    },
    VkStructureMember{
        "bufferDeviceAddress",
        &VkPhysicalDeviceVulkan12Features::bufferDeviceAddress,
    },
    VkStructureMember{
        "bufferDeviceAddressCaptureReplay",
        &VkPhysicalDeviceVulkan12Features::bufferDeviceAddressCaptureReplay,
    },
    VkStructureMember{
        "bufferDeviceAddressMultiDevice",
        &VkPhysicalDeviceVulkan12Features::bufferDeviceAddressMultiDevice,
    },
    VkStructureMember{
        "vulkanMemoryModel",
        &VkPhysicalDeviceVulkan12Features::vulkanMemoryModel,
    },
    VkStructureMember{
        "vulkanMemoryModelDeviceScope",
        &VkPhysicalDeviceVulkan12Features::vulkanMemoryModelDeviceScope,
    },
    VkStructureMember{
        "vulkanMemoryModelAvailabilityVisibilityChains",
        &VkPhysicalDeviceVulkan12Features::vulkanMemoryModelAvailabilityVisibilityChains,
    },
    VkStructureMember{
        "shaderOutputViewportIndex",
        &VkPhysicalDeviceVulkan12Features::shaderOutputViewportIndex,
    },
    VkStructureMember{
        "shaderOutputLayer",
        &VkPhysicalDeviceVulkan12Features::shaderOutputLayer,
    },
    VkStructureMember{
        "subgroupBroadcastDynamicId",
        &VkPhysicalDeviceVulkan12Features::subgroupBroadcastDynamicId,
    }// <codegen:endMemberArray>
    };

};


// <codegen:endVkPhysicalDeviceVulkan12Features>// <codegen:beginVkPhysicalDeviceVulkan13Features>

template <>
struct VkStructureTraits<VkPhysicalDeviceVulkan13Features> {
    // <codegen:beginSType>
    static constexpr VkStructureType sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_3_FEATURES;

    // <codegen:endSType>

    static constexpr const char* name = "VkPhysicalDeviceVulkan13Features";

    static constexpr std::tuple members{
    // <codegen:beginMemberArray>
        VkStructureMember{
        "pNext",
        &VkPhysicalDeviceVulkan13Features::pNext,
    },
    VkStructureMember{
        "robustImageAccess",
        &VkPhysicalDeviceVulkan13Features::robustImageAccess,
    },
    VkStructureMember{
        "inlineUniformBlock",
        &VkPhysicalDeviceVulkan13Features::inlineUniformBlock,
    },
    VkStructureMember{
        "descriptorBindingInlineUniformBlockUpdateAfterBind",
        &VkPhysicalDeviceVulkan13Features::descriptorBindingInlineUniformBlockUpdateAfterBind,
    },
    VkStructureMember{
        "pipelineCreationCacheControl",
        &VkPhysicalDeviceVulkan13Features::pipelineCreationCacheControl,
    },
    VkStructureMember{
        "privateData",
        &VkPhysicalDeviceVulkan13Features::privateData,
    },
    VkStructureMember{
        "shaderDemoteToHelperInvocation",
        &VkPhysicalDeviceVulkan13Features::shaderDemoteToHelperInvocation,
    },
    VkStructureMember{
        "shaderTerminateInvocation",
        &VkPhysicalDeviceVulkan13Features::shaderTerminateInvocation,
    },
    VkStructureMember{
        "subgroupSizeControl",
        &VkPhysicalDeviceVulkan13Features::subgroupSizeControl,
    },
    VkStructureMember{
        "computeFullSubgroups",
        &VkPhysicalDeviceVulkan13Features::computeFullSubgroups,
    },
    VkStructureMember{
        "synchronization2",
        &VkPhysicalDeviceVulkan13Features::synchronization2,
    },
    VkStructureMember{
        "textureCompressionASTC_HDR",
        &VkPhysicalDeviceVulkan13Features::textureCompressionASTC_HDR,
    },
    VkStructureMember{
        "shaderZeroInitializeWorkgroupMemory",
        &VkPhysicalDeviceVulkan13Features::shaderZeroInitializeWorkgroupMemory,
    },
    VkStructureMember{
        "dynamicRendering",
        &VkPhysicalDeviceVulkan13Features::dynamicRendering,
    },
    VkStructureMember{
        "shaderIntegerDotProduct",
        &VkPhysicalDeviceVulkan13Features::shaderIntegerDotProduct,
    },
    VkStructureMember{
        "maintenance4",
        &VkPhysicalDeviceVulkan13Features::maintenance4,
    }// <codegen:endMemberArray>
    };

};


// <codegen:endVkPhysicalDeviceVulkan13Features>// <codegen:beginVkPhysicalDeviceVulkan14Features>

template <>
struct VkStructureTraits<VkPhysicalDeviceVulkan14Features> {
    // <codegen:beginSType>
    static constexpr VkStructureType sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_4_FEATURES;

    // <codegen:endSType>

    static constexpr const char* name = "VkPhysicalDeviceVulkan14Features";

    static constexpr std::tuple members{
    // <codegen:beginMemberArray>
        VkStructureMember{
        "pNext",
        &VkPhysicalDeviceVulkan14Features::pNext,
    },
    VkStructureMember{
        "globalPriorityQuery",
        &VkPhysicalDeviceVulkan14Features::globalPriorityQuery,
    },
    VkStructureMember{
        "shaderSubgroupRotate",
        &VkPhysicalDeviceVulkan14Features::shaderSubgroupRotate,
    },
    VkStructureMember{
        "shaderSubgroupRotateClustered",
        &VkPhysicalDeviceVulkan14Features::shaderSubgroupRotateClustered,
    },
    VkStructureMember{
        "shaderFloatControls2",
        &VkPhysicalDeviceVulkan14Features::shaderFloatControls2,
    },
    VkStructureMember{
        "shaderExpectAssume",
        &VkPhysicalDeviceVulkan14Features::shaderExpectAssume,
    },
    VkStructureMember{
        "rectangularLines",
        &VkPhysicalDeviceVulkan14Features::rectangularLines,
    },
    VkStructureMember{
        "bresenhamLines",
        &VkPhysicalDeviceVulkan14Features::bresenhamLines,
    },
    VkStructureMember{
        "smoothLines",
        &VkPhysicalDeviceVulkan14Features::smoothLines,
    },
    VkStructureMember{
        "stippledRectangularLines",
        &VkPhysicalDeviceVulkan14Features::stippledRectangularLines,
    },
    VkStructureMember{
        "stippledBresenhamLines",
        &VkPhysicalDeviceVulkan14Features::stippledBresenhamLines,
    },
    VkStructureMember{
        "stippledSmoothLines",
        &VkPhysicalDeviceVulkan14Features::stippledSmoothLines,
    },
    VkStructureMember{
        "vertexAttributeInstanceRateDivisor",
        &VkPhysicalDeviceVulkan14Features::vertexAttributeInstanceRateDivisor,
    },
    VkStructureMember{
        "vertexAttributeInstanceRateZeroDivisor",
        &VkPhysicalDeviceVulkan14Features::vertexAttributeInstanceRateZeroDivisor,
    },
    VkStructureMember{
        "indexTypeUint8",
        &VkPhysicalDeviceVulkan14Features::indexTypeUint8,
    },
    VkStructureMember{
        "dynamicRenderingLocalRead",
        &VkPhysicalDeviceVulkan14Features::dynamicRenderingLocalRead,
    },
    VkStructureMember{
        "maintenance5",
        &VkPhysicalDeviceVulkan14Features::maintenance5,
    },
    VkStructureMember{
        "maintenance6",
        &VkPhysicalDeviceVulkan14Features::maintenance6,
    },
    VkStructureMember{
        "pipelineProtectedAccess",
        &VkPhysicalDeviceVulkan14Features::pipelineProtectedAccess,
    },
    VkStructureMember{
        "pipelineRobustness",
        &VkPhysicalDeviceVulkan14Features::pipelineRobustness,
    },
    VkStructureMember{
        "hostImageCopy",
        &VkPhysicalDeviceVulkan14Features::hostImageCopy,
    },
    VkStructureMember{
        "pushDescriptor",
        &VkPhysicalDeviceVulkan14Features::pushDescriptor,
    }// <codegen:endMemberArray>
    };

};


// <codegen:endVkPhysicalDeviceVulkan14Features>// <codegen:beginVkPhysicalDeviceExtendedDynamicStateFeaturesEXT>

template <>
struct VkStructureTraits<VkPhysicalDeviceExtendedDynamicStateFeaturesEXT> {
    // <codegen:beginSType>
    static constexpr VkStructureType sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_EXTENDED_DYNAMIC_STATE_FEATURES_EXT;

    // <codegen:endSType>

    static constexpr const char* name = "VkPhysicalDeviceExtendedDynamicStateFeaturesEXT";

    static constexpr std::tuple members{
    // <codegen:beginMemberArray>
        VkStructureMember{
        "pNext",
        &VkPhysicalDeviceExtendedDynamicStateFeaturesEXT::pNext,
    },
    VkStructureMember{
        "extendedDynamicState",
        &VkPhysicalDeviceExtendedDynamicStateFeaturesEXT::extendedDynamicState,
    }// <codegen:endMemberArray>
    };

};



template<typename Structure, typename Fn>
void forEachMember(Structure& structure, Fn&& fn) {
    std::apply(
        [&](const auto&... member)
        {
            (member.visit(structure, fn), ...);
        },
        VkStructureTraits<std::remove_cv_t<Structure>>::members
    );
}

/*
template<typename Structure, typename Fn>
void forEachMemberPair(const Structure& first, const Structure& second, Fn&& fn) {
    std::apply(
        [&](const auto&... member)
        {
            (std::invoke(fn, member.name, member.get(first), member.get(second)), ...);
        },
        VkStructureTraits<Structure>::members
    );
}*/

template<typename Structure, typename Fn>
void forEachMemberPair(const Structure& first, const Structure& second, Fn&& fn) {
    std::apply(
        [&](const auto&... member)
        {
            auto processMember = [&](const auto& m)
            {
                using MemberType = std::remove_cvref_t<decltype(m.get(first))>;

                if constexpr (HasVkStructureTraits<MemberType>) {
                    forEachMemberPair(m.get(first), m.get(second), fn);
                }
                else if constexpr (requires {
                    {m.get(first) != 0 } -> std::convertible_to<bool>;
                }) {
                    std::invoke(fn, m.name, m.get(first), m.get(second));
                }
            };
            (processMember(member), ...);
        },
        VkStructureTraits<Structure>::members);
}


// <codegen:endVkPhysicalDeviceExtendedDynamicStateFeaturesEXT>

#endif