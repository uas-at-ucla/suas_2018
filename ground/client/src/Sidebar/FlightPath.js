import React, { Component } from 'react';

class FlightPath extends Component {

  render() {
  	const waypointsList = this.props.homeState.waypoints.map((waypoint) =>
      <li>{waypoint.lat + ', ' + waypoint.lng + ', ' + waypoint.alt + ' ft'}</li>
    );
    
    return (
      <div>
        <p className="card-text"><b>Waypoints</b></p>
        <ol id="waypoints_list">{waypointsList}</ol>
        <button className="btn btn-outline-primary" onClick={this.addMissionWaypoints}>Add Mission Waypoints</button>
        <button className="btn btn-outline-primary">Add New Waypoint</button>
        <button className="btn btn-outline-success" onClick={this.sendGotoWaypointsCommand}>Execute!</button>
      </div>
    );
  }

  addMissionWaypoints = () => {
    let waypoints = this.props.homeState.waypoints.slice();
    let mission_waypoints = this.props.homeState.mission.mission_waypoints;
    for (let mission_waypoint of mission_waypoints) {
      let waypoint = {
        lat: mission_waypoint.latitude,
        lng: mission_waypoint.longitude,
        alt: mission_waypoint.altitude_msl,
        fromMission: true
      }
      waypoints.push(waypoint);
    }
    this.props.setHomeState({waypoints: waypoints});
  }

  sendGotoWaypointsCommand = () => {
    let commands = [];
    for (let waypoint of this.props.homeState.waypoints) {
      commands.push({
        type: 'goto',
        pos: waypoint
      });
    }
    this.props.socketEmit('execute_commands', commands);
  }
}

export default FlightPath
