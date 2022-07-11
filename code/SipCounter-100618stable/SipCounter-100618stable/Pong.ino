void Pong() {
  int oldButtonAState = buttonAState;
  int oldButtonBState = buttonBState;
  int oldButtonCState = buttonCState;

  buttonAState = digitalRead(LeftSip);
  buttonBState = digitalRead(buttonB);
  buttonCState = digitalRead(RightSip);
  
  digitalWrite (RED_LED, LOW);
  digitalWrite (GREEN_LED, LOW);

  display.setTextSize(1);
  display.setTextColor(WHITE);
  display.setCursor(0, 0);
  display.clearDisplay();

  // check for ball-paddle collision (crude)
  if ((ball.x - ball.r <= paddle.centerx + paddle.width / 2 + 1)
      and (ball.y <= paddle.centery + paddle.height / 2)
      and (ball.y >= paddle.centery - paddle.height / 2)) {
    ball.dx *= -1.5;
    if (ball.dx > 9) {  //limit speed increase
      ball.dx = 10;
    }
    score += 1;
    Blink (RED_LED, 25, 1);
    if (score > max_score) {
      max_score = score;
    }
  }

  // check for ball-bottom collision
  if (ball.x <= ball.r) {
    ball.x = 100;
    if (score > max_score) {
      max_score = score;
    }
    score = 0;
    ball.dx = 2;
    Blink (RED_LED, 100, 4);
    Blink (GREEN_LED, 100, 4);
  }

  // update
  ball.update();
  paddle.update();

  // draw
  display.drawLine(0, 0, 109, 0, WHITE);
  display.drawLine(0, 31, 109, 31, WHITE);
  display.drawLine(109, 0, 109, 31, WHITE);
  display.setRotation(1);
  display.setCursor(0, 0);
  display.print(score);
  display.setCursor(0, 9);
  display.print("HI:");
  display.print(max_score);
  //display.print(ball.dx);
  display.setRotation(0);
  ball.draw();
  paddle.draw();
  display.display();
}
