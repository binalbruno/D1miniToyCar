R"rawliteral(
<!DOCTYPE html>
<html>
  <head>
    <title>PMUT Demo car controller</title>
    <style>
      body {
        display: flex;
        justify-content: center;
        align-items: center;
        height: 100vh;
        margin: 0;
        font-family: Arial, sans-serif;
      }

      .controller-container {
        display: grid;
        grid-template-columns: repeat(3, 200px); /* Three columns */
        grid-gap: 20px;
      }

      .btn {
        width: 200px;
        height: 200px;
        background-color: #007bff;
        border: none;
        border-radius: 10px;
        touch-action: manipulation;
        cursor: pointer;
        font-size: 50px;
        color: white;
        display: flex;
        justify-content: center;
        align-items: center;
      }

      .btn:active {
        background-color: #0056b3;
      }

      /* Placeholder div to leave empty space */
      .empty {
        width: 200px;
        height: 200px;
      }
    </style>
    <script>
      function sendCommand(command) {
        fetch('/' + command)
          .then(response => response.text())
          .then(data => console.log(data))
          .catch(error => console.error('Error:', error));
      }

      function onPress(direction) {
        sendCommand(direction + '-press');
      }

      function onRelease(direction) {
        sendCommand(direction + '-release');
      }
    </script>
  </head>
  <body>
    <div class="controller-container">
      <!-- First row -->
      <button class="btn" 
        onmousedown="onPress('left')" 
        onmouseup="onRelease('left')" 
        ontouchstart="onPress('left')" 
        ontouchend="onRelease('left')">LEFT</button>
        
      <button class="btn" 
        onmousedown="onPress('right')" 
        onmouseup="onRelease('right')" 
        ontouchstart="onPress('right')" 
        ontouchend="onRelease('right')">RIGHT</button>
        
      <button class="btn" 
        onmousedown="onPress('forward')" 
        onmouseup="onRelease('forward')" 
        ontouchstart="onPress('forward')" 
        ontouchend="onRelease('forward')">DRIVE</button>

      <!-- Second row -->
      <div class="empty"></div> <!-- Empty space -->
      <div class="empty"></div> <!-- Empty space -->
      <button class="btn" 
        onmousedown="onPress('backward')" 
        onmouseup="onRelease('backward')" 
        ontouchstart="onPress('backward')" 
        ontouchend="onRelease('backward')">BACK</button>
    </div>
  </body>
</html>
)rawliteral";