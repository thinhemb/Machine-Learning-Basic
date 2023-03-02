// Copyright (C) 2018-2021 Intel Corporation
// SPDX-License-Identifier: Apache-2.0
//

/**
 * @deprecated Use vpu/hddl_config.hpp instead.
 * @brief A header that defines advanced related properties for VPU plugins.
 * These properties should be used in SetConfig() and LoadNetwork() methods of plugins
 *
 * @file hddl_plugin_config.hpp
 */

#pragma once

#include "ie_api.h"
#include "ie_plugin_config.hpp"

//
// Options
//

/**
 * @def VPU_HDDL_CONFIG_KEY(name)
 * @brief Shortcut for defining VPU HDDL configuration key
 */
#define VPU_HDDL_CONFIG_KEY(name) InferenceEngine::VPUConfigParams::_CONFIG_KEY(VPU_HDDL_##name)
/**
 * @def VPU_HDDL_CONFIG_VALUE(name)
 * @brief Shortcut for defining VPU HDDL configuration value
 */
#define VPU_HDDL_CONFIG_VALUE(name) InferenceEngine::VPUConfigParams::VPU_HDDL_##name

#define DECLARE_VPU_HDDL_CONFIG_KEY(name) DECLARE_CONFIG_KEY(VPU_HDDL_##name)
#define DECLARE_VPU_HDDL_CONFIG_VALUE(name) DECLARE_CONFIG_VALUE(VPU_HDDL_##name)

//
// Metrics
//

/**
 * @def VPU_HDDL_METRIC(name)
 * @brief Shortcut for defining VPU HDDL metric
 */
#define VPU_HDDL_METRIC(name) METRIC_KEY(VPU_HDDL_##name)
#define DECLARE_VPU_HDDL_METRIC(name, ...)  DECLARE_METRIC_KEY(VPU_HDDL_##name, __VA_ARGS__)

namespace InferenceEngine {

namespace Metrics {

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_NUM instead
* @brief Metric to get a int of the device number, String value is METRIC_VPU_HDDL_DEVICE_NUM
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_NUM instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_NUM, int);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_NAME instead
* @brief Metric to get a std::vector<std::string> of device names, String value is METRIC_VPU_HDDL_DEVICE_NAME
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_NAME instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_NAME, std::vector<std::string>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_THERMAL instead
* @brief  Metric to get a std::vector<float> of device thermal, String value is METRIC_VPU_HDDL_DEVICE_THERMAL
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_THERMAL instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_THERMAL, std::vector<float>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_ID instead
* @brief  Metric to get a std::vector<uint32> of device ids, String value is METRIC_VPU_HDDL_DEVICE_ID
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_ID instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_ID, std::vector<unsigned int>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_SUBCLASS instead
* @brief  Metric to get a std::vector<int> of device subclasses, String value is METRIC_VPU_HDDL_DEVICE_SUBCLASS
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_SUBCLASS instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_SUBCLASS, std::vector<int>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_MEMORY_TOTAL instead
* @brief  Metric to get a std::vector<uint32> of device total memory, String value is METRIC_VPU_HDDL_MEMORY_TOTAL
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_MEMORY_TOTAL instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_MEMORY_TOTAL, std::vector<unsigned int>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_MEMORY_USED instead
* @brief  Metric to get a std::vector<uint32> of device used memory, String value is METRIC_VPU_HDDL_DEVICE_MEMORY_USED
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_MEMORY_USED instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_MEMORY_USED, std::vector<unsigned int>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_UTILIZATION instead
* @brief  Metric to get a std::vector<float> of device utilization, String value is METRIC_VPU_HDDL_DEVICE_UTILIZATION
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_UTILIZATION instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_UTILIZATION, std::vector<float>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_STREAM_ID instead
* @brief  Metric to get a std::vector<std::string> of stream ids, String value is METRIC_VPU_HDDL_DEVICE_STREAM_ID
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_STREAM_ID instead")
DECLARE_VPU_HDDL_METRIC(STREAM_ID, std::vector<std::string>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_TAG instead
* @brief  Metric to get a std::vector<std::string> of device tags, String value is METRIC_VPU_HDDL_DEVICE_TAG
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_TAG instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_TAG, std::vector<std::string>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_GROUP_ID instead
* @brief  Metric to get a std::vector<int> of group ids, String value is METRIC_VPU_HDDL_GROUP_ID
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_GROUP_ID instead")
DECLARE_VPU_HDDL_METRIC(GROUP_ID, std::vector<int>);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_GROUP_USING_NUM instead
* @brief  Metric to get a int number of device be using for group, String value is METRIC_VPU_HDDL_DEVICE_GROUP_USING_NUM
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_GROUP_USING_NUM instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_GROUP_USING_NUM, int);

/**
* @deprecated Use InferenceEngine::METRIC_HDDL_DEVICE_TOTAL_NUM instead
* @brief  Metric to get a int number of total device, String value is METRIC_VPU_HDDL_DEVICE_TOTAL_NUM
*/
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::METRIC_HDDL_DEVICE_TOTAL_NUM instead")
DECLARE_VPU_HDDL_METRIC(DEVICE_TOTAL_NUM, int);

}  // namespace Metrics

namespace VPUConfigParams {

/**
 * @deprecated Use InferenceEngine::HDDL_GRAPH_TAG instead
 * @brief [Only for HDDLPlugin]
 * Type: Arbitrary non-empty string. If empty (""), equals no set, default: "";
 * This option allows to specify the number of MYX devices used for inference a specific Executable network.
 * Note: Only one network would be allocated to one device.
 * The number of devices for the tag is specified in the hddl_service.config file.
 * Example:
 * "service_settings":
 * {
 *     "graph_tag_map":
 *     {
 *         "tagA":3
 *     }
 * }
 * It means that an executable network marked with tagA will be executed on 3 devices
 */
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::HDDL_GRAPH_TAG instead")
DECLARE_VPU_HDDL_CONFIG_KEY(GRAPH_TAG);

/**
 * @deprecated Use InferenceEngine::HDDL_STREAM_ID instead
 * @brief [Only for HDDLPlugin]
 * Type: Arbitrary non-empty string. If empty (""), equals no set, default: "";
 * This config makes the executable networks to be allocated on one certain device (instead of multiple devices).
 * And all inference through this executable network, will be done on this device.
 * Note: Only one network would be allocated to one device.
 * The number of devices which will be used for stream-affinity must be specified in hddl_service.config file.
 * Example:
 * "service_settings":
 * {
 *     "stream_device_number":5
 * }
 * It means that 5 device will be used for stream-affinity
 */
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::HDDL_STREAM_ID instead")
DECLARE_VPU_HDDL_CONFIG_KEY(STREAM_ID);

/**
 * @deprecated Use InferenceEngine::HDDL_DEVICE_TAG instead
 * @brief [Only for HDDLPlugin]
 * Type: Arbitrary non-empty string. If empty (""), equals no set, default: "";
 * This config allows user to control device flexibly. This config gives a "tag" for a certain device while
 * allocating a network to it. Afterward, user can allocating/deallocating networks to this device with this "tag".
 * Devices used for such use case is controlled by a so-called "Bypass Scheduler" in HDDL backend, and the number
 * of such device need to be specified in hddl_service.config file.
 * Example:
 * "service_settings":
 * {
 *     "bypass_device_number": 5
 * }
 * It means that 5 device will be used for Bypass scheduler.
 */
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::HDDL_DEVICE_TAG instead")
DECLARE_VPU_HDDL_CONFIG_KEY(DEVICE_TAG);

/**
 * @deprecated Use InferenceEngine::HDDL_BIND_DEVICE instead
 * @brief [Only for HDDLPlugin]
 * Type: "YES/NO", default is "NO".
 * This config is a sub-config of DEVICE_TAG, and only available when "DEVICE_TAG" is set. After a user load a
 * network, the user got a handle for the network.
 * If "YES", the network allocated is bind to the device (with the specified "DEVICE_TAG"), which means all afterwards
 * inference through this network handle will be executed on this device only.
 * If "NO", the network allocated is not bind to the device (with the specified "DEVICE_TAG"). If the same network
 * is allocated on multiple other devices (also set BIND_DEVICE to "False"), then inference through any handle of these
 * networks may be executed on any of these devices those have the network loaded.
 */
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::HDDL_BIND_DEVICE instead")
DECLARE_VPU_HDDL_CONFIG_KEY(BIND_DEVICE);

/**
 * @deprecated Use InferenceEngine::HDDL_RUNTIME_PRIORITY instead
 * @brief [Only for HDDLPlugin]
 * Type: A signed int wrapped in a string, default is "0".
 * This config is a sub-config of DEVICE_TAG, and only available when "DEVICE_TAG" is set and "BIND_DEVICE" is "False".
 * When there are multiple devices running a certain network (a same network running on multiple devices in Bypass Scheduler),
 * the device with a larger number has a higher priority, and more inference tasks will be fed to it with priority.
 */
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::HDDL_RUNTIME_PRIORITY instead")
DECLARE_VPU_HDDL_CONFIG_KEY(RUNTIME_PRIORITY);

/**
 * @deprecated Use InferenceEngine::HDDL_USE_SGAD instead
 * @brief [Only for HDDLPlugin]
 * Type: "YES/NO", default is "NO".
 * SGAD is short for "Single Graph All Device". With this scheduler, once application allocates 1 network, all devices
 * (managed by SGAD scheduler) will be loaded with this graph. The number of network that can be loaded to one device
 * can exceed one. Once application deallocates 1 network from device, all devices will unload the network from them.
 */
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::HDDL_USE_SGAD instead")
DECLARE_VPU_HDDL_CONFIG_KEY(USE_SGAD);

/**
 * @deprecated Use InferenceEngine::HDDL_GROUP_DEVICE instead
 * @brief [Only for HDDLPlugin]
 * Type: A signed int wrapped in a string, default is "0".
 * This config gives a "group id" for a certain device when this device has been reserved for certain client, client
 * can use this device grouped by calling this group id while other client can't use this device
 * Each device has their own group id. Device in one group shares same group id.
 */
INFERENCE_ENGINE_DEPRECATED("Use InferenceEngine::HDDL_GROUP_DEVICE instead")
DECLARE_VPU_HDDL_CONFIG_KEY(GROUP_DEVICE);

}  // namespace VPUConfigParams

}  // namespace InferenceEngine
