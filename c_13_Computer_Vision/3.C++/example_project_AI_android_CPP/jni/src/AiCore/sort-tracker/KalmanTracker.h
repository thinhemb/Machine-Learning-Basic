///////////////////////////////////////////////////////////////////////////////
// KalmanTracker.h: KalmanTracker Class Declaration

#ifndef KALMAN_H
#define KALMAN_H 2

#include "opencv2/video/tracking.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace std;
using namespace cv;

#define StateType Rect_<float>


// This class represents the internel state of individual tracked objects observed as bounding box.
class KalmanTracker
{
public:
	KalmanTracker()
	{
		init_kf(StateType());
		m_time_since_update = 0;
		m_hits = 0;
		m_hit_streak = 0;
		m_age = 0;
		m_id = kf_count;
		//kf_count++;
	}
	KalmanTracker(StateType initRect)
	{
		init_kf(initRect);
		m_time_since_update = 0;
		m_hits = 0;
		m_hit_streak = 0;
		m_age = 0;
		m_id = kf_count;
		kf_count++;
		if(kf_count >= ULONG_MAX)
			kf_count = 0;
		
	}

	~KalmanTracker()
	{
		m_history.clear();
	}

	Rect2f predict();
	void update(Rect2f stateMat);
	Point2f get_point();
	Rect2f get_state();
	Rect2f get_rect_xysr(float cx, float cy, float s, float r);
	
	static unsigned long kf_count;

	int m_time_since_update;
	int m_hits;
	unsigned long long  m_hit_streak;
	int m_age;
	unsigned long m_id;

private:
	void init_kf(Rect2f stateMat);

	cv::KalmanFilter kf;
	cv::Mat measurement;

	std::vector<Rect2f> m_history;

	int max_age = 10;
};




#endif