#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include <math.h> 
#include <ctime>
#include "ros/ros.h"
#include "geometry_msgs/Point32.h"
#include "sensor_msgs/PointCloud2.h"
#include "tf/transform_listener.h"

#include <move_base_msgs/MoveBaseAction.h>
#include <actionlib/client/simple_action_client.h>
#include <sys/time.h>

#include <nav_msgs/GetMap.h>
#include "geometry_msgs/Pose2D.h"


#include "actionlib_msgs/GoalStatus.h"
#include "actionlib_msgs/GoalID.h"
#include "nav_msgs/OccupancyGrid.h"

#include "projector2d.h"
#include "exploration/fake_projector.h"

#include "g2o/types/slam2d/se2.h"
#include "frontier_detector.h"
#include "paths_rollout.h"
#include "srrg_types/defs.h"

typedef actionlib::SimpleActionClient<move_base_msgs::MoveBaseAction> MoveBaseClient;


class GoalPlanner {

public:

	
	GoalPlanner(MoveBaseClient *ac, FakeProjector *projector, FrontierDetector *frontierDetector,Vector2f laserOffset = {0.0, 0.5}, int minThresholdSize = 10);

	bool requestOccupancyMap();
	bool requestCloudsUpdate();

	void publishGoal(PoseWithInfo goalPose, std::string frame);

	void waitForGoal();

	std::string getActionServerStatus();

	cv::Mat getImageMap();

	float getResolution();

	Vector2fVector getAbortedGoals();

	void setUnknownCellsCloud(Vector2fVector* cloud);
	void setOccupiedCellsCloud(Vector2fVector* cloud);

protected:

	bool isGoalReached(Vector2fVector cloud);
	int computeVisiblePoints(Vector3f robotPose, Vector2f laserOffset,Vector2fVector cloud, int numInterestingPoints);


	FakeProjector *_projector;
	Vector2f _laserOffset;
	FrontierDetector *_frontierDetector;

	float _mapResolution;

	float _xyThreshold = 0.25;

	PoseWithInfo _goal;
	int _minUnknownRegionSize;


	Vector2fVector* _unknownCellsCloud;
	Vector2fVector* _occupiedCellsCloud;

	Vector2fVector _abortedGoals;

	std::string _fixedFrameId;

	cv::Mat* _occupancyMap;
	
	ros::NodeHandle _nh;
	ros::ServiceClient _mapClient;
	MoveBaseClient* _ac;

	tf::TransformListener _tfListener;
	tf::StampedTransform _tfMapToBase;



};