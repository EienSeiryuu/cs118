.DEFAULT_GOAL := router
server: server.cpp 
	g++ -c server.cpp
client: client.cpp 
	g++ -c client.cpp
udp_driver: udp_driver.cpp
	g++ udp_driver.cpp server.cpp client.cpp -o udp_driver
clean:
	rm -f *.o udp_driver
router:
	g++ -std=c++11 my-router.cpp -o my-router
