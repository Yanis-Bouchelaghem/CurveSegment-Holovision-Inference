#pragma once
#include <string>
#include <vector>
#include <opencv2/core.hpp>
#include <random>
#include "RandomWalk.h"

class VideoProcessor
{
public:
	VideoProcessor(const std::vector<cv::Mat>& video, int videoIndex,
				   const RandomWalk& randomWalk, float segmentThreshold, bool visualizeSegments = true);
	void GenerateImagesFromSegments(std::string outputFolder, int segmentTarget, int segmentLength) const;
private:
	cv::Mat GenerateImageFromSegment(const std::vector<Vec2<int>>& curveSegment) const;
	cv::Mat DrawSegmentOnFirstFrame(const std::vector<Vec2<int>>& curveSegment) const;
	cv::Mat DrawSegmentOnFrame(const std::vector<Vec2<int>>& curveSegment, cv::Mat frame) const;
private:
	const std::vector<cv::Mat>& video;
	const int videoIndex;
	const float segmentThreshold;
	const RandomWalk& randomWalk;
	const bool visualizeSegments;
};