// ScoutBotics 10/27/2018 Level 2 competition

// These are line following and turning solutions for the 
// tiles called from modeFive()

void Tile1()
{
  if (MyBot.foundLeft) {
    MyBot.turn('L');
    MyBot.followLine(3);
    MyBot.turn('R');
    MyBot.followLine(3);
    MyBot.turn('R');
    MyBot.followLine(3);
    MyBot.turn('L');
    MyBot.followLine(3);
  } else {
    MyBot.turn('R');
    MyBot.followLine(3);
    MyBot.turn('L');
    MyBot.followLine(3);
    MyBot.turn('L');
    MyBot.followLine(3);
    MyBot.turn('R');
    MyBot.followLine(3);
  }
}

void Tile2()
{
  if (MyBot.foundRight) {
    MyBot.turn('L');
    MyBot.followLine(3);
    MyBot.turn('R');
    MyBot.followLine(3);
    MyBot.followLine(3);
  } else {
    MyBot.followLine(3);
    MyBot.turn('L');
    MyBot.followLine(3);
    MyBot.turn('R');
    MyBot.followLine(3);
  }
}

void Tile3()
{
  if (MyBot.foundLeft) {
    MyBot.followLine(3);
    MyBot.followLine(3);
    if (MyBot.foundEnd) {
      MyBot.turn('L'); // Turn around
      MyBot.followLine(3);
      MyBot.turn('R');
      MyBot.followLine(3);
      MyBot.turn('R');
      MyBot.followLine(3);
    }
  } else {
    MyBot.followLine(3);
    MyBot.followLine(3);
    if (MyBot.foundEnd) {
      MyBot.turn('L'); // Turn around
      MyBot.followLine(3);
      MyBot.turn('L');
      MyBot.followLine(3);
      MyBot.turn('L');
      MyBot.followLine(3);
    }
  }
}

void Tile4()
{
  if (MyBot.foundLeft) {
    MyBot.turn('L');
    MyBot.followLine(3);
    MyBot.followLine(3);
  } else {
    MyBot.followLine(3);
    MyBot.turn('R');
    MyBot.followLine(3);
  }
}

void Tile5()
{
  if (MyBot.foundLeft) {
    MyBot.followLine(3);
    MyBot.turn('L');
    MyBot.followLine(3);
    MyBot.followLine(3);
  } else {
    MyBot.followLine(3);
    MyBot.turn('R');
    MyBot.followLine(3);
    MyBot.followLine(3);
  }
}

void Tile6()
{
  if (MyBot.foundLeft) {
    if (MyBot.foundRight) {
      MyBot.turn('L');
      MyBot.followLine(3);
    } else {
      MyBot.turn('L');
      MyBot.followLine(3);
      MyBot.followLine(3);
    }
    if (MyBot.foundEnd) {
      MyBot.turn('L'); // Turn around
      MyBot.followLine(3);
      MyBot.followLine(3);
      MyBot.turn('L');
      MyBot.followLine(3);
      MyBot.turn('L');
      MyBot.followLine(3);
    }
  } else {
    if (MyBot.foundForward) {
      MyBot.turn('R');
      MyBot.followLine(3);
      if (MyBot.foundEnd) {
        MyBot.turn('L'); // Turn around
        MyBot.followLine(3);
        MyBot.turn('R');
        MyBot.followLine(3);
        MyBot.turn('R');
        MyBot.followLine(3);
      }
    } else {
      MyBot.turn('R');
      MyBot.followLine(3);
      MyBot.followLine(3);
      if (MyBot.foundEnd) {
        MyBot.turn('L'); // Turn around
        MyBot.followLine(3);
        MyBot.turn('L');
        MyBot.followLine(3);
        MyBot.turn('L');
        MyBot.followLine(3);
      }
    }
  }
}



