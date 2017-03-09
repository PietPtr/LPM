#include <stdio.h>
#include <iostream>
#include <vector>

void read_routes(void);
void read_lookup(void);
void ip2human(unsigned int ip);
int lookup_ip(unsigned int ip);
void add_route(unsigned int p, int prefix_length, int port_number);

struct route {
	unsigned int ip;
	int prefix_length;
	int port_number;
};

std::vector<route> routes;

int main(void) {
	// TODO: Request token from student assistant
	puts("s1234567_abcde");
	fflush(stdout);

	read_routes();
	read_lookup();

	while(true) {

	}

	return 0;
}

void add_route(unsigned int ip, int prefix_length, int port_number) {
	// TODO: Store route to be used for later use in lookup_ip() function
	//std::cout << ip << "|" << prefix_length << "|" << port_number << '\n';
	route r;
	r.ip = ip;
	r.prefix_length = prefix_length;
	r.port_number = port_number;
	routes.push_back(r);

}

bool is_in_range(unsigned int ip_range, int prefix_length, unsigned int ip) {
	unsigned int lower_bound = ip_range & (0xFFFFFFFF << (32 - prefix_length));
	unsigned int upper_bound = ip_range | (0xFFFFFFFF >> (prefix_length));

	// std::cout << "\n" << prefix_length << "\n";
	// ip2human(lower_bound);
	// ip2human(upper_bound);

	return ip > lower_bound && ip < upper_bound;
}

int lookup_ip(unsigned int ip) {
	// TODO: Lookup IP in stored data from add_route function,
	//       returns port number (or -1 for no route found).
	for (unsigned int i = 0; i < routes.size(); i++) {
		if (is_in_range(routes[i].ip, routes[i].prefix_length, ip)) {
			return routes[i].port_number;
		}
	}
	return -1;
}

void ip2human(unsigned int ip) {
	unsigned int a, b, c, d;

	a = (ip >> 24) & 0xff;
	b = (ip >> 16) & 0xff;
	c = (ip >>  8) & 0xff;
	d =  ip        & 0xff;

	printf("%i.%i.%i.%i\n", a, b, c, d);
}

void read_routes(void) {
	FILE *f = fopen("routes.txt", "r");
	char s[27];

	while (fgets(s, 27, f)) {
		unsigned int a, b, c, d, ip;
		int pl, pn;

		sscanf(s, "%i.%i.%i.%i/%i %i", &a, &b, &c, &d, &pl, &pn);
		ip = (a<<24) | (b<<16) | (c<<8) | d;

		add_route(ip, pl, pn);
	}

	fclose(f);
}

void read_lookup(void) {
	FILE *f = fopen("lookup.txt", "r");
	char s[17];

	while (fgets(s, 17, f)) {
		unsigned int a, b, c, d, ip;
		int pn;

		sscanf(s, "%i.%i.%i.%i", &a, &b, &c, &d);
		ip = (a<<24) | (b<<16) | (c<<8) | d;

		pn = lookup_ip(ip);
		printf("%i\n", pn);
	}

	fclose(f);
}
