// Get a pop up when form is 'submitted' and clear the fields

$("#submit").click(() => {
  alert("Your message has been sent to tobyhutchinson@gmail.com :)")
  $("#subject").html("")
  $("#message").html("")
  $("#email").html("")
})

const map = document.getElementById("map")

$("#lewes").click(() => {

  navigator.geolocation.getCurrentPosition((position) => {
    let lat = position.coords.latitude  
    let long = position.coords.longitude

    map.src = "https://www.google.com/maps/embed?pb=!1m26!1m12!1m3!1d2497194.110471738!2d-3.015232186300779!3d52.32206071365408!2m3!1f0!2f0!3f0!3m2!1i1024!2i768!4f13.1!4m11!3e2!4m3!3m2!1d" + lat + "!2d" + long + "!4m5!1s0x47df6025a6ac1ca7%3A0xd5d74cafdefe7924!2sLewes!3m2!1d50.873872!2d0.00878!5e0!3m2!1sen!2suk!4v1651582318251!5m2!1sen!2suk"

  })
})