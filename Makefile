tsp: clean
	@g++ main.cpp -o $@ -lm
clean:
	@rm -rf tsp