
int data = 8;

int libex_get_data() {
	return data;
}

void libex_set_data(int val) {
	data = val;
}

int * libex_get_data_addr() {
	return &data;
}
