import React, { Component } from 'react';
import './Dashboard.css'

class Dashboard extends Component {

  render() {
    const waypointsList = this.props.home.state.waypoints.map((waypoint) =>
      <li>{waypoint.lat + ', ' + waypoint.lng + ', ' + waypoint.alt + ' ft'}</li>
    );

    return (
      <div className="Dashboard">
        <div id="sidebar_content">
          <div className="card text-white">
            <h4 className="card-header">Missions</h4>
            <div className="card-body">
              <p className="card-text">Mission 1</p>
            </div>
          </div>
        </div>
        <div className="card text-white">
          <h4 className="card-header">Flight Path</h4>
          <div className="card-body">
            <p className="card-text"><b>Waypoints</b></p>
            <ol id="waypoints_list">{waypointsList}</ol>
            <button className="btn btn-outline-primary" onClick={this.addMissionWaypoints}>Add Mission Waypoints</button>
            <button className="btn btn-outline-primary">Add New Waypoint</button>
            <button className="btn btn-outline-success" onClick={this.sendGotoWaypointsCommand}>Execute!</button>
          </div>
        </div>
        <div className="card text-white">
          <h4 className="card-header">Received Images</h4>
          <div className="card-body">
            <p className="card-text">Feature coming... eventually</p>
          </div>
        </div>
      </div>
    );
  }

  addMissionWaypoints = () => {
    var waypoints = this.props.home.state.waypoints.slice();
    var mission_waypoints = this.props.home.state.mission.mission_waypoints;
    for (var mission_waypoint of mission_waypoints) {
      var waypoint = {
        lat: mission_waypoint.latitude,
        lng: mission_waypoint.longitude,
        alt: mission_waypoint.altitude_msl,
        fromMission: true
      }
      waypoints.push(waypoint);
    }
    this.props.home.setState({waypoints: waypoints});
  }

  sendGotoWaypointsCommand = () => {
    var commands = [];
    for (var waypoint of this.props.home.state.waypoints) {
      commands.push({
        type: 'goto',
        pos: waypoint
      });
    }
    this.props.app.socketEmit('execute_commands', commands);
  }
}

export default Dashboard
