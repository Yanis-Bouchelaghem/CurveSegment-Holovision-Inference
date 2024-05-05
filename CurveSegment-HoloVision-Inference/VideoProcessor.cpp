#include "VideoProcessor.h"
#include <opencv2/highgui.hpp>
#include <filesystem>

VideoProcessor::VideoProcessor(const std::vector<cv::Mat>& video,
							   int videoIndex,
							   const RandomWalk& randomWalk,
							   float segmentThreshold,
							   bool visualizeSegments)
	:
	video(video),
	videoIndex(videoIndex),
	randomWalk(randomWalk),
	segmentThreshold(segmentThreshold),
	visualizeSegments(visualizeSegments)
{
	assert(video[0].rows == randomWalk.GetHeight()); //If assertion triggers : The dimensions of the frames does not match
	assert(video[0].cols == randomWalk.GetWidth());  //the dimensions given in the settings file.
}

void VideoProcessor::GenerateImagesFromSegments(std::string outputFolder, int segmentTarget, int segmentLength) const
{
	//Create the target folders if they do not exist
	std::filesystem::create_directory(outputFolder);
	std::filesystem::create_directory(outputFolder+ "/video_" + std::to_string(videoIndex));

	//Initialize the frame on which we will draw the segments for visualization purposes
	cv::Mat visualizationFrame = video[0].clone();
	std::stringstream segmentOutputPath;
	segmentOutputPath << outputFolder << "/video_" + std::to_string(videoIndex) << "/!segments_visualization.png";
	//As long as we haven't reached the target numbers, keep generating segments
	int segmentCount = 0;

	while (segmentCount < segmentTarget)
	{
		//Generate a randomwalk
		std::vector<Vec2<int>> curveSegment = randomWalk.GenerateCurveSegment(segmentLength);
		//Generate the evolution image in the correctly labeled folder
		cv::Mat generatedImage = GenerateImageFromSegment(curveSegment);
		std::stringstream outputPath;
		outputPath << outputFolder << "/video_" + std::to_string(videoIndex) << "/" << segmentCount << ".png";
		cv::imwrite(outputPath.str(), generatedImage);
		if (visualizeSegments)
		{
			DrawSegmentOnFrame(curveSegment, visualizationFrame);
		}
		++segmentCount;
	}
	if (visualizeSegments)
	{
		//Write the segment visualisation on disk
		cv::imwrite(segmentOutputPath.str(), visualizationFrame);
	}
}


cv::Mat VideoProcessor::GenerateImageFromSegment(const std::vector<Vec2<int>>& curveSegment) const
{
	//Generate image from curve segment (y = curve segment size, x = number of frames in the video)
	cv::Mat curveSegmentResult(curveSegment.size(), video.size(), CV_8UC3);
	for (int i = 0; i < curveSegment.size(); ++i)
	{
		Vec2 position = curveSegment[i];
		for (int frameIndex = 0; frameIndex < video.size(); ++frameIndex)
		{
			curveSegmentResult.at<cv::Vec3b>(i, frameIndex) = video[frameIndex].at<cv::Vec3b>(position.y, position.x);
		}
	}
	return curveSegmentResult;
}

cv::Mat VideoProcessor::DrawSegmentOnFirstFrame(const std::vector<Vec2<int>>& curveSegment) const
{
	cv::Mat firstFrame = video[0].clone();
	for (const Vec2<int>& position : curveSegment)
	{
		firstFrame.at<cv::Vec3b>(position.y, position.x) = cv::Vec3b(0, 0, 255);
	}
	return firstFrame;
}

cv::Mat VideoProcessor::DrawSegmentOnFrame(const std::vector<Vec2<int>>& curveSegment, cv::Mat frame) const
{
	for (const Vec2<int>& position : curveSegment)
	{
		frame.at<cv::Vec3b>(position.y, position.x) = cv::Vec3b(0, 0, 255);
	}
	return frame;
}
