import React, { Component } from 'react';
import * as THREE from 'three';

import './AttitudeIndicator.css';
import NavballImg from './navball.png';

class Navball extends Component {
    componentDidMount() {
        const WIDTH = 200;
        const HEIGHT = WIDTH;

        this.scene = new THREE.Scene();

        this.camera = new THREE.PerspectiveCamera(41 , 1 , 1 , 2000);
        this.scene.add(this.camera);
        this.camera.position.set(0 , 0 , 120);
        this.camera.lookAt(this.scene.position);


        this.renderer = new THREE.WebGLRenderer({ antialias: true, alpha: true });
        this.renderer.setSize(WIDTH, HEIGHT);
        document.getElementById("navball").appendChild(this.renderer.domElement);

        var light = new THREE.AmbientLight( 0xffffff , 1.5 );;
        this.scene.add(light);


        var sphereGeom = new THREE.SphereGeometry(40 , 48 , 32);
        var texture = new THREE.TextureLoader().load(NavballImg);
        var material = new THREE.MeshPhongMaterial({map: texture});
        this.ball = new THREE.Mesh(sphereGeom , material);
        this.ball.position.set(0, -3.14/2, 0);
        this.scene.add(this.ball);

        this.start();
    }


    start = () => {
        if (!this.frameId) {
          this.frameId = requestAnimationFrame(this.animate)
        }
    }


    stop = () => {
        cancelAnimationFrame(this.frameId)
    }


    animate = () => {
       this.ball.rotation.x = this.props.data["navX"];             //pitch
       this.ball.rotation.y = this.props.data["navY"] - 3.1415926; //yaw
       this.ball.rotation.z = this.props.data["navZ"];             //roll
       this.renderScene()
       this.frameId = window.requestAnimationFrame(this.animate)
    }


    renderScene = () => {
      this.renderer.render(this.scene, this.camera)
    }



    render() {
        return(
            <div className="AttitudeIndicator">
                <div className="navball-background"></div>
                <div className="navball" id="navball" ref={(mount) => {this.mount = mount}}></div>
            </div>
        );
    }
}


export default Navball;