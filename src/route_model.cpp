#include "route_model.h"
#include <iostream>

RouteModel::RouteModel(const std::vector<std::byte> &xml) : Model(xml) {
    int i = 0;
    for(Model::Node node : this->Nodes()){
        mNodes.push_back(Node(i, this, node));
      	i++;
    }
    CreateNodeToRoadHashmap();
}
float RouteModel::Node::distance(RouteModel::Node node){
  return std::sqrt(std::pow(node.x - x, 2) + std::pow(node.y - y, 2));
}

/**
 * What we do here is that we get the vector of roads
 * Go through them and for all of them that are not footways
 * we  map node ids to their respective roads
 */ 
void RouteModel::CreateNodeToRoadHashmap(){
    for(const Model::Road& road : Roads()){
        if(road.type != Road::Type::Footway){
            for(int idx : Ways()[road.way].nodes){
              if(node_to_road.find(idx) == node_to_road.end()){
                node_to_road[idx] = std::vector<const Model::Road* >();
              }
                node_to_road[idx].push_back(&road);
            }
        }
    }
}
//Returns a map of node to roads
std::unordered_map<int, std::vector<const Model::Road*>>& RouteModel::GetNodeToRoadHashmap(){
    return node_to_road;
}
std::vector<RouteModel::Node>& RouteModel::SNodes(){
    return mNodes;
}
RouteModel::Node* RouteModel::Node::FindNeighbor(std::vector<int> node_indices){
    Node* closest_node = nullptr;
    for(int node_index : node_indices){
        Node node = parent_model->SNodes()[node_index];
      	if(this->distance(node) != 0 && !node.visited){
           if(closest_node== nullptr || this->distance(node) < this->distance(*closest_node)){
              closest_node = &parent_model->SNodes()[node_index];
           }
        }

    }
    return closest_node;
}
void RouteModel::Node::FindNeighbors(){
    for(auto& road : parent_model->node_to_road[this->index]){
        std::vector<int> node_indices = parent_model->Ways()[road->way].nodes;
        RouteModel::Node* new_node = this->FindNeighbor(node_indices);
        if(new_node){
            this->neighbors.push_back(new_node);
        }
    }
}
/**
 * We use the FindClosestNode method to get the 
 * approximately closest node in our list of nodes
*/
RouteModel::Node& RouteModel::FindClosestNode(double x, double y){
    Node temp;
    temp.x = x;
    temp.y = y;
    float min_dist = std::numeric_limits<float>::max();
  	float dist;
  	int closest_idx;
    for(const auto road : Roads()){
        if(road.type != Road::Type::Footway){
            for(int node_idx : Ways()[road.way].nodes){
                dist = temp.distance(SNodes()[node_idx]);
                if(dist < min_dist){
                  	min_dist = dist;
                    closest_idx = node_idx;
                }
            }
        }
    }
    return SNodes()[closest_idx];
}
