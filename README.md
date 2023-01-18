# Satellite-Antenna-Rotator
## currently untested on hardware

 Adapted from Viorel Racoviteannu's project: https://projecthub.arduino.cc/viorelracoviteanu/a5a0d407-f1ca-4c13-980c-0217aa87384a
 - Uses 2 servos that have 180 degrees of movement
 - Receives commands over serial and searches the strings for `AZxxx.x` and/or `ELxxx.x`, where x is an integer (0-9)
 - Make sure the servos can handle the weight of the antenna. Mounting it so it is balanced helps (mount Yagi's in the middle, use counterweights, etc.). With strong enough servos, it should be able to hold a real antenna.
 - Elevation servo will use full 180 degrees to make up for the azimuth's lack of 360, so make sure the antenna is not obstructed at any position
