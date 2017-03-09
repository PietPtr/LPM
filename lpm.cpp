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

enum CompareResult { LOWER, IN, HIGHER };

int main(void) {
	// TODO: Request token from student assistant
	puts("s1776894_4qcw2");
	fflush(stdout);

	read_routes();
	read_lookup();

	// while(true) {
	//
	// }

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

CompareResult is_in_range(int index, unsigned int ip) {
	unsigned int lower_bound = routes[index].ip &
							   (0xFFFFFFFF << (32 - routes[index].prefix_length));
	unsigned int upper_bound = routes[index].ip |
							   (0xFFFFFFFF >> (routes[index].prefix_length));

	// std::cout << "\n" << routes[index].prefix_length << "\n";
	// ip2human(lower_bound);
	// ip2human(upper_bound);
	// std::cout << lower_bound << " " << upper_bound << "\n";
	// ip2human(routes[index].ip);

	if (ip >= lower_bound && ip <= upper_bound)
		return IN;
	if (lower_bound < ip)
		return LOWER;
	if (upper_bound > ip)
		return HIGHER;
}

int lookup_ip_binary(unsigned int ip, int lower_index, int higher_index) {
	//std::cout << lower_index << " > " << higher_index << " = " << (lower_index > higher_index) << "\n";
	if (lower_index > higher_index) {
		int timeout = 0;
		while (!(is_in_range(lower_index - 1, ip) == IN) && lower_index >= 0) {
			lower_index--;

			timeout++;
			if (timeout > 2000) {
				return -1;
			}
		}
		return lower_index - 1;
	}

	int m = (lower_index + higher_index) / 2;

	CompareResult result = is_in_range(m, ip);
	//std::cout << "result: " << result << "\n\n";
	//ip2human(routes[m].ip);

	if (result == LOWER)
		return lookup_ip_binary(ip, m + 1, higher_index);
	else if (result == HIGHER)
		return lookup_ip_binary(ip, lower_index, m - 1);
	else
		return m;
}

int lookup_ip(unsigned int ip) {
	// TODO: Lookup IP in stored data from add_route function,
	//       returns port number (or -1 for no route found).
	// for (unsigned int i = 0; i < routes.size(); i++) {
	// 	if (is_in_range(i, ip)) {
	// 		return routes[i].port_number;
	// 	}
	// }
	//
	// ip2human(ip);
	// std::cout << ip << "\n";

	int binary_result = lookup_ip_binary(ip, 0, routes.size() - 1);

	if (binary_result == -1) {
		return -1;
	}

	int specific_index = binary_result;
	// while (is_in_range(specific_index + 1, ip) == IN && (specific_index + 1) < routes.size()) {
	// 	specific_index++;
	// }

	unsigned int lower_bound = routes[specific_index].ip &
			   (0xFFFFFFFF << (32 - routes[specific_index].prefix_length));
	int distance  = abs(ip - lower_bound);
	int found_index = binary_result;

	while(true)
	{
		specific_index++;

		lower_bound = routes[specific_index].ip &
				   (0xFFFFFFFF << (32 - routes[specific_index].prefix_length));
		int next_distance = abs(ip - lower_bound);

		if(next_distance <= distance)
		{
			distance = next_distance;
			if(is_in_range(specific_index, ip) == IN)
			{
				found_index = specific_index;
			}
		}
		else
		{
			specific_index--;
			break;
		}
	}

	return routes[found_index].port_number;
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
