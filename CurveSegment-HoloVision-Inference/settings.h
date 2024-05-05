#pragma once

namespace settings
{
	inline constexpr int minimumFrameCount = 33; //33 is the minimum number of frames in the dataset for the origin passport videos.
	inline constexpr int frameWidth = 615;
	inline constexpr int frameHeight = 432;
	inline constexpr int segmentLength = 64;
	inline constexpr float segmentThreshold = 0.3f; //The threshold that needs to be reached for a segment to be considered hologram
	inline constexpr int targetSegmentCountPerVideo = 5000;
}