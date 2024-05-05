#include <iostream>
#include "settings.h"
#include "DatasetLoader.h"
#include "RandomWalk.h"
#include "VideoProcessor.h"

int main()
{
	using namespace settings;
	DatasetLoader datasetLoader("inference_dataset");
	datasetLoader.TruncateFrameCountTo(minimumFrameCount);
	RandomWalk randomWalk(frameHeight, frameWidth);
	std::cout << "Found " << datasetLoader.GetVideoCount() << " videos in the dataset...\n";
	std::cout << "Generating :\n" << targetSegmentCountPerVideo << "images per video.\n\n";
	constexpr bool generateSegmentVisualization = true;
	for (int i = 0; i < datasetLoader.GetVideoCount(); ++i)
	{
		std::cout << "Processing video " << i << "...\n";
		auto video = datasetLoader.LoadVideo(i, cv::IMREAD_COLOR);
		VideoProcessor videoProcessor(video,
			i,
			randomWalk,
			segmentThreshold,
			generateSegmentVisualization);
		videoProcessor.GenerateImagesFromSegments("generatedImages", targetSegmentCountPerVideo, segmentLength);
	}

	return 0;
}