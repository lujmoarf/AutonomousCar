/*void padExample(Control *sender, int value) {
  switch (value) {
  case P_LEFT_DOWN:
    Serial.println("left down");
    if (direction == 0){
      turn_forward_left();
    } else if (direction == 1){
      turn_backward_left();
    }
    ESPUI.print(statusLabelId, "Left Down");
    break;

  case P_LEFT_UP:
    Serial.println("left up");
    ESPUI.print(statusLabelId, "Left Up");

    if (direction == 0){
      move_forward_m1();
    } else if (direction == 1){
      move_backward_m1();
    }
    
    break;

  case P_RIGHT_DOWN:
    Serial.println("right down");
    ESPUI.print(statusLabelId, "Right Down");

    if (direction == 0){
      turn_forward_right();
    } else if (direction == 1){
      turn_backward_right();
    }
    
    break;

  case P_RIGHT_UP:
    Serial.println("right up");
    ESPUI.print(statusLabelId, "Right Up");
    if (direction == 0){
      move_forward_m1();
    } else if (direction == 1){
      move_backward_m1();
    }
    break;

  case P_FOR_DOWN:
    Serial.println("forward down");
    ESPUI.print(statusLabelId, "Forward Down");
    move_forward_m1();
    break;

  case P_FOR_UP:
    Serial.println("forward up");
    ESPUI.print(statusLabelId, "Forward Up");
    move_forward_m1();
    break;

  case P_BACK_DOWN:
    Serial.println("backward down");
    ESPUI.print(statusLabelId, "Back down");
    move_backward_m1();
    break;

  case P_BACK_UP:
    Serial.println("backward up");
    ESPUI.print(statusLabelId, "Back Up");
    move_backward_m1();
    break;

  case P_CENTER_DOWN:
    Serial.println("center down");
    ESPUI.print(statusLabelId, "Center Down");
    stop_m1();
    break;

  case P_CENTER_UP:
    Serial.println("center up");
    ESPUI.print(statusLabelId, "Center Up");
    break;
  }

}

void slider(Control *sender, int type) {
  Serial.print("Slider: ID: ");
  Serial.print(sender->id);
  // Like all Control Values in ESPUI slider values are Strings. To use them as int simply do this:
  int sliderValueWithOffset = sender->value.toInt() + 155;
  Serial.print("Value with offset=");
  Serial.println(sliderValueWithOffset);
  dutyCycle= sliderValueWithOffset;
  ledcWrite(pwmChannel, dutyCycle);
}
*/
