// Function to send a message to the Pebble using AppMessage API
function sendMessage(msg, dst, arw) {
  Pebble.sendAppMessage({"directions": msg, "distance": dst, "arrow": arw});
  
	// PRO TIP: If you are sending more than one message, or a complex set of messages, 
	// it is important that you setup an ackHandler and a nackHandler and call 
	// Pebble.sendAppMessage({ /* Message here */ }, ackHandler, nackHandler), which 
	// will designate the ackHandler and nackHandler that will be called upon the Pebble 
	// ack-ing or nack-ing the message you just sent. The specified nackHandler will 
	// also be called if your message send attempt times out.
}

// Called when JS is ready
Pebble.addEventListener("ready",
							function(e) {
                sendMessage("Turn right on Juli-Lyn Ln", "349ft", 1);
              });