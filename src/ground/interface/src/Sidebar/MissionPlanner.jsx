import React, { Component } from "react";
import {
  FormControl,
  FieldGroup,
  FormGroup,
  ControlLabel,
} from "react-bootstrap";
import { InputGroup, InputGroupAddon, InputGroupText, Input } from 'reactstrap';

import {
  SortableContainer,
  SortableElement,
  arrayMove
} from "react-sortable-hoc";

const SortableItem = SortableElement(({ command, myIndex, self }) => (
  <tr
    className="command_row"
    onClick={event => self.onCommandClick(myIndex, event)}
  >
    <td>{myIndex + 1}</td>
    <td>
      <FormGroup controlId="formControlsSelect" className="type_select">
        {self.commandTypeOptions(myIndex, command)}
      </FormGroup>
    </td>
    <td>
      <InputGroup controlId="lat">
        <InputGroupAddon addonType="prepend">
          <InputGroupText>lat</InputGroupText>
        </InputGroupAddon>
        <Input placeholder="10" type="number" width="5" bsSize="small"/>
      </InputGroup>
    </td>
    <td>
      <InputGroup controlId="lng">
        <InputGroupAddon addonType="prepend">
          <InputGroupText>lng</InputGroupText>
        </InputGroupAddon>
        <Input placeholder="10" type="number" width="5" bsSize="small"/>
      </InputGroup>
    </td>
    <td>
      <InputGroup controlId="alt">
        <InputGroupAddon addonType="prepend">
          <InputGroupText>alt</InputGroupText>
        </InputGroupAddon>
        <Input placeholder="10" type="number" width="5" bsSize="small"/>
      </InputGroup>
    </td>
    <td>{self.extras(myIndex, command)}</td>
  </tr>
));

const SortableList = SortableContainer(({ commands, self }) => {
  return (
    <div className="scrollbar">
      <table id="commandList">
        <tbody>
          {commands.map((command, index) => (
            <SortableItem
              key={`item-${index}`}
              index={index}
              command={command}
              myIndex={index}
              self={self}
            />
          ))}
        </tbody>
      </table>
    </div>
  );
});

class MissionPlanner extends Component {
  render() {
    return (
      <SortableList
        commands={this.props.homeState.commands}
        onSortEnd={this.onSortEnd}
        self={this}
        transitionDuration={200}
        distance={2}
      />
    );
  }

  onSortEnd = ({ oldIndex, newIndex }) => {
    if (oldIndex !== newIndex) {
      this.props.setHomeState({
        commands: arrayMove(this.props.homeState.commands, oldIndex, newIndex)
      });
    }
  };

  onCommandTypeChange(index, event) {
    let commands = this.props.homeState.commands.slice();
    commands[index].options.command_type = event.target.value;
    this.props.setHomeState({
      commands: commands,
      dontRedrawCommands: true
    });
  }

  onCommandNameChange(index, event) {
    let commands = this.props.homeState.commands.slice();
    commands[index].name = event.target.value;
    this.props.setHomeState({
      commands: commands,
      dontRedrawCommands: true
    });
  }

  onCommandAltChange(index, event) {
    let commands = this.props.homeState.commands.slice();
    commands[index].options.alt = Number(event.target.value);
    this.props.setHomeState({
      commands: commands,
      dontRedrawCommands: true
    });
  }

  onLineSepChange(index, event) {
    let commands = this.props.homeState.commands.slice();
    commands[index].options.line_sep = Number(event.target.value);
    this.props.setHomeState({ commands: commands });
  }

  onCommandClick(index, event) {
    if (event.target.tagName === "TD") {
      this.props.setHomeState({ focusedCommand: index });
    }
  }

  commandTypeOptions(index, command) {
    if (command.options.command_type === "survey") {
      return (
        <FormControl
          componentClass="select"
          placeholder="select"
          value={command.options.command_type}
          onChange={event => this.onCommandTypeChange(index, event)}
        >
          <option>survey</option>
        </FormControl>
      );
    } else {
      return (
        <FormControl
          componentClass="select"
          placeholder="select"
          value={command.options.command_type}
          onChange={event => this.onCommandTypeChange(index, event)}
        >
          <option>goto</option>
          <option>jump</option>
          <option>bomb</option>
          <option>off-axis</option>
        </FormControl>
      );
    }
  }

  extras(index, command) {
    if (command.options.command_type === "survey") {
      return (
        <div>
          Line Separation:
          <input
            className="line_sep_input"
            type="number"
            value={command.options.line_sep}
            onChange={event => this.onLineSepChange(index, event)}
          />
        </div>
      );
    } else if (command.options.command_type === "off-axis") {
      if (
        this.props.homeState.mission &&
        this.props.homeState.mission.off_axis_odlc_pos
      ) {
        let pos = this.props.homeState.mission.off_axis_odlc_pos;
        command.options.off_axis_pos = {
          lat: pos.latitude,
          lng: pos.longitude
        };
        return (
          <div>
            Off-Axis Pos: <font color="green">Known</font>
          </div>
        );
      } else {
        return (
          <div>
            Off-Axis Pos: <font color="red">Unknown</font>
          </div>
        );
      }
    }
  }
}

export default MissionPlanner;
