# header - ROS header containing timestamp information.
Header header

# id - unique integer ID number used as a key to find this job in the database (auto-generated by the database)
uint32 id

# name - name of the job that is meaningful to the user
string name

# description - description of the job
string description

# part_id - integer ID used to identify the part associated with this job
uint32 part_id

# tasks - Array of tasks associated with this job
opp_msgs/ToolPath[] paths
