#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <errno.h>

struct Address{
	int id;
	int set;
	char *name;
	char *email;
};

struct Database {
	int max_data;
	int max_rows;
	struct Address **rows;
};

struct Connection {
	FILE *file;
	struct Database *db;
};


void Database_close(struct Connection *conn){
	if(conn){
		size_t i = 0;
		if(conn->file)
			fclose(conn->file);
		for(i = 0; i < conn->db->max_rows; i++){
			if(conn->db->rows[i]){
				free(conn->db->rows[i]->name);
				free(conn->db->rows[i]->email);
				free(conn->db->rows[i]);
			}
		}
		if(conn->db)
			free(conn->db);
		free(conn);
	}
}

void die(char *message, struct Connection *conn){
	if(errno){
		perror(message);
	}else{
		printf("ERROR: %s\n", message);	
	}
	
	if(conn){
		Database_close(conn);
	}
	exit(1);
}

void Database_load(struct Connection *conn){
	int rc = fread(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file);
	if(rc != 1) die("Failed to load MAX_ROWS.",conn);

	rc = fread(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file);
	if(rc != 1) die("Failed to load MAX_DATA.",conn);

	conn->db->rows = malloc(conn->db->max_rows * sizeof(struct Address*));
	

	for(int i=0;i<conn->db->max_rows;i++) {
	struct Address *addr = malloc(sizeof(struct Address));
	rc = fread(&(addr->id), sizeof(int),1,conn->file);
	if(rc != 1) die("Failed to load address->id",conn);

	rc = fread(&(addr->set), sizeof(int),1,conn->file);
	if(rc != 1) die("Failed to load address->set",conn);

	addr->name = malloc(conn->db->max_data * sizeof(char));
	addr->email= malloc(conn->db->max_data * sizeof(char));

	rc = fread(addr->name,sizeof(char),conn->db->max_data,conn->file); 
	if(rc != conn->db->max_data) die("Failed to load address->name",conn);

	rc = fread(addr->email,sizeof(char),conn->db->max_data,conn->file);
	if(rc != conn->db->max_data) die("Failed to load address->email",conn);

	conn->db->rows[i] = addr;
	}
}

struct Connection *Database_open(const char *filename, char mode){
	struct Connection *conn = malloc(sizeof(struct Connection));
	if(!conn)
		die("Memory error", conn);
	conn->db = malloc(sizeof(struct Database));
	if(!conn->db)
		die("Memory error", conn);
	if(mode == 'c'){
		conn->file = fopen(filename, "w");
	}else{
		conn->file = fopen(filename, "r+");
		if(conn-> file){
			Database_load(conn);
		}
	}

	return conn;
}

void Database_write(struct Connection *conn){
	rewind(conn -> file);
	int rc = fwrite(&conn->db->max_rows, sizeof(conn->db->max_rows), 1, conn->file);
	if(rc != 1)
		die("Failed to write max_data",conn);
	rc = fwrite(&conn->db->max_data, sizeof(conn->db->max_data), 1, conn->file);
	if(rc != 1)
		die("Failed to write max_rows",conn);
/*	rc = fwrite(conn->db, sizeof(struct Database), 1, conn->file);
	if(rc != 1){
		die("Failed to write the database", conn);
	}*/
	for(int i=0; i<conn->db->max_rows;i++){
		struct Address *addr = conn->db->rows[i];
		rc = fwrite(&addr->id, sizeof(addr->id), 1, conn->file);
		if(rc!=1)
			die("Failed to write id", conn);
		rc = fwrite(&addr->set, sizeof(addr->set), 1, conn->file);
		if(rc!=1)
			die("Failed to write set", conn);
		rc = fwrite(addr->name, sizeof(char), conn->db->max_data, conn->file);
		if(rc!=conn->db->max_data)
			die("Failed to write addr->name", conn);
		rc = fwrite(addr->email, sizeof(char),conn->db->max_data, conn->file);
		if(rc!=conn->db->max_data)
			die("Failed to write email", conn);

	}
	rc = fflush(conn->file);
	if(rc == -1){	
		die("Cannot flush the database",conn);
	}
}

void Database_create(struct Connection *conn, int max_data, int max_rows){
	int i=0;
	conn->db->max_data = max_data;
	conn->db->max_rows = max_rows;
	conn->db->rows = malloc(max_rows * sizeof(struct Address*));
	//conn->db->rows = malloc(sizeof(struct Address));
	for(i = 0; i < max_rows; i++){
		struct Address *addr = malloc(sizeof(struct Address));
		addr->id = i;
		addr->set = 0;
		addr->name = malloc(max_data * sizeof(char));
		addr->email = malloc(max_data * sizeof(char));
		
		memset(addr->name, '0', max_data * sizeof(char));
		memset(addr->email, '0', max_data * sizeof(char));
		memset(addr->name, '-', max_data * sizeof(char) -1);
		memset(addr->email, '_', max_data * sizeof(char) -1);
	
		conn->db->rows[i] = addr;
	}
}

void Database_set(struct Connection *conn,int id, const char *name, const char *email){
	
	struct Address *addr = conn->db->rows[id];
	if(addr->set){
		die("Record is already in use", conn);
	}
	addr->set = 1;
	char *res = strncpy(addr->name, name,conn->db->max_data);
	if(!res)
		die("Name copy failed",conn);
	res = strncpy(addr->email, email, conn->db->max_data);
	if(!res)
		die("Email copy failed", conn);
}

void Database_get(struct Connection *conn, int id){
	size_t i;
	for(i = 0; i < conn->db->max_rows; i++){
		struct Address *cur = conn->db->rows[i];
		if(cur->id == id){
			printf("%d, %s, %s\n", cur->id, cur->name, cur->email);
			return;
		}
	}
}

void Database_list(struct Connection *conn){
	size_t i;
	for(i=0; i< conn->db->max_rows; i++){
		struct Address *addr = conn->db->rows[i];
		if(addr->set){
			printf("id: %d, name: %s, email: %s\n", addr->id, addr->name, addr->email);
		}
	}
}
int main(int argc, char *argv[]){	
	if(argc < 5) die("Not enough parameters, usage is ./db <db filename> <Max_data> <Max_rows> <action>  [operation params]", NULL);
	int id = 0;
	char *filename = argv[1];
	char action = argv[4][0];
	int max_data = atoi(argv[2]);
	int max_rows = atoi(argv[3]);
	struct Connection *conn = Database_open(filename, action);
	if(argc > 5) id = atoi(argv[5]);
	if(id > max_rows) die("There's not that many records!", conn);

	switch(action){
		case 'c': 
			Database_create(conn, max_data, max_rows);
			Database_write(conn);
			break;
		case 's': 
			if(argc != 8) die("Need id, name, email to set", conn);
			Database_set(conn, id, argv[6],argv[7]);
			Database_write(conn);
			break;
		case 'l':
			Database_list(conn);
			break;
		default:
			die("Invalid operation", conn);
	}
	Database_close(conn);
}
