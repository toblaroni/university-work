// Contains code for the game on the home page.

window.onload = () => {

   const light = "#ffc107"

   const links = ["#0d6efd", "#dc3545", "#fd7e14", "#198754"]

   const canvas = document.getElementById("snake")
   const pressKeyMessage = document.getElementById("anykey")
   const bg = document.getElementById("bg-image")

   if(window.innerWidth < 576){
      canvas.remove()
      pressKeyMessage.remove()
      bg.style.display = 'block'
   } else {
      pressKeyMessage.style.display = 'block'
   }

   // Set width and height to full screen   
   canvas.width = window.innerWidth
   canvas.height = window.innerHeight

   // Check that canvas is supported
   if(canvas.getContext){
      ctx = canvas.getContext("2d")
   } else {
      alert("We're sorry, but you cannot enjoy this website to the fullest since your browser doesn't support HTML canvas tags...")
      return
   }

   let width = canvas.width
   let height = canvas.height
   
   let snakeGirth = 15
   let currentLength = 5

   let currentPosition = [snakeGirth * 10, snakeGirth * 10]
   let tail = []

   let direction 

   // Set background to black
   ctx.fillStyle = "black"
   ctx.fillRect(0, 0, width, height)

   // Take in keyboard input
   document.onkeydown = (event) => {
      let key = event.keyCode
      if(key == 37 && direction != "right"){
         direction = "left"
      } else if (key == 39 && direction != "left"){
         direction = "right"
      }
      else if (key == 38 && direction != "down"){
         direction = "up"
      } else if (key == 40 && direction != "up"){
         direction = "down"
      }
      if(key >= 37 && key <= 40){
         pressKeyMessage.remove()
      }
   }

   function draw(){
      // Draw Snake
      ctx.fillStyle = light
      ctx.fillRect(currentPosition[0], currentPosition[1], snakeGirth, snakeGirth)
   }

   // Keep snake moving
   setInterval(() => {
      if (currentPosition[0] < 0){
         currentPosition[0] = width
      } else if (currentPosition[1] < 0){
         currentPosition[1] = height
      } else if (currentPosition[0] > width){
         currentPosition[0] = 0
      } else if (currentPosition[1] > height){
         currentPosition[1] = 0
      }

      checkForLink()
      switch(direction){
         case "up":
            currentPosition[1] -= snakeGirth
            draw()
            break
         case "down":
            currentPosition[1] += snakeGirth
            draw()
            break
         case "left":
            currentPosition[0] -= snakeGirth 
            draw()
            break
         case "right":
            currentPosition[0] += snakeGirth
            draw()
            break
      }

      if (direction != undefined){
         tail.push(currentPosition[0], currentPosition[1])
      }
      let snakeLength = tail.length/2

      if(snakeLength > currentLength && direction != undefined){
         let rmItemX = tail.shift()
         let rmItemY = tail.shift()
         ctx.fillStyle = "black"
         ctx.fillRect(rmItemX, rmItemY, snakeGirth, snakeGirth)
      }
      
   }, 50)

   function positionLinks(){
      // Apply some random position for each link
      for(let i = 0; i < 4; ++ i){
         ctx.fillStyle = links[i]
         let linkX =  Math.round((Math.random() * (width - snakeGirth * 20)) / snakeGirth) * snakeGirth
         let linkY =  Math.round((Math.random() * (height - snakeGirth * 20)) / snakeGirth) * snakeGirth
         ctx.fillRect(linkX, linkY, snakeGirth, snakeGirth)
      }
   }
   positionLinks()

   function checkForLink(){
      // Function to check the colour of pixel in front of the snake
      let imageData
      if(tail.length == 10){
         if(direction == "right"){
            imageData = ctx.getImageData(tail[8] + snakeGirth, tail[9] + snakeGirth/2, 1, 1)

         } else if(direction == "left"){
            imageData = ctx.getImageData(tail[8] - snakeGirth, tail[9] + snakeGirth/2, 1, 1)

         } else if(direction == "up"){
            imageData = ctx.getImageData(tail[8] + snakeGirth/2, tail[9] - snakeGirth, 1, 1)

         } else {
            imageData = ctx.getImageData(tail[8] + snakeGirth/2, tail[9] + snakeGirth, 1, 1)

         }

         // If the colour of the pixel matches one of the link colours go to that page.
         let pixelColor = imageData.data[0]
         
         if(pixelColor == 13 || pixelColor == 12){
            document.location.href = "contact.html"
         } else if (pixelColor == 220 || pixelColor == 221 || pixelColor == 219){
            document.location.href = "about.html"
         } else if (pixelColor == 253 || pixelColor == 254 || pixelColor == 252){
            document.location.href = "graphics.html"
         } else if (pixelColor == 25 || pixelColor == 24 || pixelColor == 26){
            document.location.href = "photo.html"
         }
      }
   }
}