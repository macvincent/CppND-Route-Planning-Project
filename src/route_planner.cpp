#include "route_planner.h"
#include <algorithm>

RoutePlanner::RoutePlanner(RouteModel &model, float start_x, float start_y, float end_x, float end_y): m_Model(model) {
    //The m_Model variable is the model for our map
    m_Model = model;
    //We multiply by 0.01 to scale to 100
    start_x *= 0.01;
    start_y *= 0.01;
    end_x *= 0.01;
    end_y *= 0.01;
    startNode = &m_Model.FindClosestNode(start_x, start_y);
    endNode = &m_Model.FindClosestNode(end_x, end_y);

}
float RoutePlanner::GetDistance(){
    return distance;
}
std::vector<RouteModel::Node> RoutePlanner::ConstructFinalPath(RouteModel::Node* node){
    std::vector<RouteModel::Node> pathFound;
    distance = 0.0f;
  	RouteModel::Node parent;
    while(node->parent != nullptr){
        pathFound.push_back(*node);
      	parent = *(node->parent);
      	distance += node->distance(parent);
      	std::cout << distance << std::endl;
      	node = node->parent;
    }
  	pathFound.push_back(*node);
    distance *= m_Model.MetricScale();
    return pathFound;
}
void RoutePlanner::AStarSearch(){
    std::cout << "start at " << startNode->x << ' ' <<  startNode->y << std::endl;
  	std::cout << "end   at " << endNode->x << ' ' << endNode->y << std::endl;
//   endNode->parent = startNode;
//   m_Model.path = ConstructFinalPath(endNode);
  	startNode->visited = true;
  	open_list.push_back(startNode);
    RouteModel::Node *current_node = nullptr;
  	while(open_list.size() > 0){
    	current_node = NextNode();
      	double distance = current_node->distance(*endNode);
      	if( distance == 0){
        	m_Model.path = ConstructFinalPath(current_node);
          	return;
        }
        AddNeighbors(current_node);
    }
}
float RoutePlanner::calculateHValue(RouteModel::Node* node){
   return node->distance(*endNode);
}
//  	std::vector<RouteModel::Node*> open_list;
bool compare(const RouteModel::Node* a, const RouteModel::Node* b){
	return (a->g_value + a->h_value) > (b->g_value + b->h_value);
}
RouteModel::Node* RoutePlanner::NextNode(){
    std::sort(open_list.begin(), open_list.end(), compare);
    RouteModel::Node* temp = open_list.back();
    open_list.pop_back();
  	return temp;
}
void RoutePlanner::AddNeighbors(RouteModel::Node* curr){
    curr->FindNeighbors();
  	for(auto neighbor : curr->neighbors){
      	neighbor->parent = curr;
    	neighbor->g_value = curr->g_value + curr->distance(*neighbor);
      	neighbor->h_value = calculateHValue(neighbor);
      	open_list.push_back(neighbor);
      	neighbor->visited = true;
    }
}