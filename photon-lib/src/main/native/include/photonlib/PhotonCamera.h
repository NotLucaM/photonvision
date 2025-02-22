/*
 * MIT License
 *
 * Copyright (c) 2022 PhotonVision
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * furnished to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */

#pragma once

#include <memory>
#include <string>

#include <networktables/NetworkTable.h>
#include <networktables/NetworkTableEntry.h>
#include <networktables/NetworkTableInstance.h>
#include <wpi/deprecated.h>

#include "photonlib/PhotonPipelineResult.h"

namespace photonlib {

enum LEDMode : int { kDefault = -1, kOff = 0, kOn = 1, kBlink = 2 };

/**
 * Represents a camera that is connected to PhotonVision.ß
 */
class PhotonCamera {
 public:
  /**
   * Constructs a PhotonCamera from a root table.
   *
   * @param instance The NetworkTableInstance to pull data from. This can be a
   * custom instance in simulation, but should *usually* be the default
   * NTInstance from {@link NetworkTableInstance::getDefault}
   * @param cameraName The name of the camera, as seen in the UI.
   * over.
   */
  explicit PhotonCamera(std::shared_ptr<nt::NetworkTableInstance> instance,
                        const std::string& cameraName);

  /**
   * Constructs a PhotonCamera from the name of the camera.
   * @param cameraName The nickname of the camera (found in the PhotonVision
   * UI).
   */
  explicit PhotonCamera(const std::string& cameraName);

  /**
   * Returns the latest pipeline result.
   * @return The latest pipeline result.
   */
  PhotonPipelineResult GetLatestResult() const;

  /**
   * Toggles driver mode.
   * @param driverMode Whether to set driver mode.
   */
  void SetDriverMode(bool driverMode);

  /**
   * Returns whether the camera is in driver mode.
   * @return Whether the camera is in driver mode.
   */
  bool GetDriverMode() const;

  /**
   * Request the camera to save a new image file from the input
   * camera stream with overlays.
   * Images take up space in the filesystem of the PhotonCamera.
   * Calling it frequently will fill up disk space and eventually
   * cause the system to stop working.
   * Clear out images in /opt/photonvision/photonvision_config/imgSaves
   * frequently to prevent issues.
   */
  void TakeInputSnapshot(void);

  /**
   * Request the camera to save a new image file from the output
   * stream with overlays.
   * Images take up space in the filesystem of the PhotonCamera.
   * Calling it frequently will fill up disk space and eventually
   * cause the system to stop working.
   * Clear out images in /opt/photonvision/photonvision_config/imgSaves
   * frequently to prevent issues.
   */
  void TakeOutputSnapshot(void);

  /**
   * Allows the user to select the active pipeline index.
   * @param index The active pipeline index.
   */
  void SetPipelineIndex(int index);

  /**
   * Returns the active pipeline index.
   * @return The active pipeline index.
   */
  int GetPipelineIndex() const;

  /**
   * Returns the current LED mode.
   * @return The current LED mode.
   */
  LEDMode GetLEDMode() const;

  /**
   * Sets the LED mode.
   * @param led The mode to set to.
   */
  void SetLEDMode(LEDMode led);

  /**
   * Returns whether the latest target result has targets.
   * This method is deprecated; {@link PhotonPipelineResult#hasTargets()} should
   * be used instead.
   * @deprecated This method should be replaced with {@link
   * PhotonPipelineResult#HasTargets()}
   * @return Whether the latest target result has targets.
   */
  WPI_DEPRECATED(
      "This method should be replaced with PhotonPipelineResult::HasTargets()")
  bool HasTargets() const { return GetLatestResult().HasTargets(); }

 protected:
  std::shared_ptr<nt::NetworkTable> mainTable;
  std::shared_ptr<nt::NetworkTable> rootTable;
  nt::NetworkTableEntry rawBytesEntry;
  nt::NetworkTableEntry driverModeEntry;
  nt::NetworkTableEntry inputSaveImgEntry;
  nt::NetworkTableEntry outputSaveImgEntry;
  nt::NetworkTableEntry pipelineIndexEntry;
  nt::NetworkTableEntry ledModeEntry;
  nt::NetworkTableEntry versionEntry;

  std::string path;

  mutable Packet packet;

 private:
  void VerifyVersion() const;
};

}  // namespace photonlib
