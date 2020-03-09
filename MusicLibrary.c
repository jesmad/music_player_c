#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#define STRMAX 40
#define PLAYLISTMAX 1024

struct Song
{
	char title[STRMAX];
	char artist[STRMAX];
	int year_published;
};

struct Song music_library[PLAYLISTMAX];
//struct Song music_library_from_file[PLAYLISTMAX];
//struct Song matched_songs[PLAYLISTMAX];
//int number_of_matched_songs = 0;
int current_number_of_songs = 0;
char chosen_song[STRMAX];
//int index_binary_search;

FILE *ifp;
FILE *ofp;
char *inputFile;
char *outputFile;

//------------------------ADD_SONG_TO_LIBRARY-------------------------------------
void ask_what_title(struct Song *track)
{
	printf("Title: ");
	//fgets(track->title, STRMAX, stdin);
	//scanf("%s", &(track->title));
	scanf(" %[^\n]", &(track->title));
	printf("\n");
}

void ask_what_artist(struct Song *track)
{
	printf("Artist: ");
	scanf(" %[^\n]", &(track->artist));
	//fgets(track->artist, STRMAX, stdin);
	//scanf("%s", &(track->artist));
	printf("\n");
}

void ask_what_year(struct Song *track)
{
	printf("Year Published: ");
	scanf(" %d", &(track->year_published));
	printf("\n");
}

struct Song ask_song_info()
{
	struct Song desired_track;
	ask_what_title(&desired_track);
	ask_what_artist(&desired_track);
	ask_what_year(&desired_track);
	return desired_track;
}

void crunch_up_from_index(int index, struct Song track)
{
	for (int j = current_number_of_songs; j > index; --j) //maybe use index
		music_library[j] = music_library[j - 1];

	music_library[index] = track;
}

void sort_playlist(struct Song track)
{
	for (int i = 0; i < current_number_of_songs; ++i)
	{
		if (strcmp(track.title, music_library[i].title) < 0)
		{
			crunch_up_from_index(i, track);
			return;
		}
	}
	music_library[current_number_of_songs] = track;
}

void add_song_to_array(struct Song track)
{
	if (current_number_of_songs == 0)
	{
		music_library[0] = track;
	}
	else
	{
		sort_playlist(track);
	}
	++current_number_of_songs;
}

void add_song_to_MusicLibrary()
{
	struct Song desired_track = ask_song_info();
	add_song_to_array(desired_track);
}
//-----------------------------PRINT_MUSIC_LIBRARY--------------------------------
void print_MusicLibrary()
{
	for (int i = 0; i < current_number_of_songs; ++i)
		printf("%d Title: %s, Artist: %s, Year Published: %d \n", i + 1, music_library[i].title, music_library[i].artist, music_library[i].year_published);
}
//-------------------------------------------------------------------------------

//-----------------------------REMOVE_SONG_FROM_MUSICLIBRARY_BY_NAME-------------

void what_song_to_remove()
{
	printf("WHICH SONG DO YOU WANT TO REMOVE?: ");
	scanf(" %[^\n]", &chosen_song); //chosen song is a global variable
	printf("\n");
}

int find_index_of_song_with_name(char remove_song[])
{
	for (int i = 0; i < current_number_of_songs; ++i)
	{
		if (strcmp(remove_song, music_library[i].title) == 0)
			return i;
	}
	return -1;
}

void crunch_down_from_index(int index)
{
	for (int i = index; i < current_number_of_songs; ++i)
		music_library[i] = music_library[i + 1];

	--current_number_of_songs;
}

void ask_song_find_index_delete()
{
	while (1)
	{
		int index = find_index_of_song_with_name(chosen_song);
		if (index == -1)
			return;
		crunch_down_from_index(index);
	}
}

void song_existence(int index)
{
	if (index == -1)
	{
		printf("SONG IS NOT IN YOUR LIBRARY\n");
		return;
	}
	crunch_down_from_index(index);
	ask_song_find_index_delete();
}

void remove_song_from_MusicLibrary_by_name()
{
	what_song_to_remove();
	int index = find_index_of_song_with_name(chosen_song);
	song_existence(index);
}
//-----------------------------------------------------------------------------

//------------------------------------LOAD_SONG-------------------------------

void ask_song_to_load()
{
	printf("WHAT SONG DO YOU WANT TO SEARCH FOR?: ");
	scanf(" %[^\n]", &chosen_song);
	printf("\n");
}

struct Song binary_search() //EXCEPTION
{
	int left_bound = 0;
	int right_bound = current_number_of_songs - 1;

	while (left_bound != right_bound)
	{

		int mid_index = (left_bound + right_bound) / 2;
		if (strcmp(chosen_song, music_library[mid_index].title) == 0)
		{
			return music_library[mid_index];
		}
		else if (strcmp(chosen_song, music_library[mid_index].title) < 0)
		{
			right_bound = mid_index;
			if ((right_bound - 1) == left_bound)
				return music_library[left_bound];
		}
		else if (strcmp(chosen_song, music_library[mid_index].title) > 0)
		{
			left_bound = mid_index;
			if ((left_bound + 1) == right_bound)
				return music_library[right_bound];
		}
	}
	return music_library[left_bound];
}

void search_and_print_song()
{
	struct Song search_song = binary_search();
	printf("Title: %s, Artist: %s, Year Published: %d\n", search_song.title, search_song.artist, search_song.year_published);
}

void load_desired_song()
{
	ask_song_to_load();
	if (find_index_of_song_with_name(chosen_song) == -1)
	{
		printf("SONG IS NOT IN YOUR LIBRARY\n");
		return;
	}
	search_and_print_song();
}

//--------------------------------------STORE_MUSICLIBRARY_FILE---------------------------------
void store_MusicLibrary() //EXCEPTION
{
	outputFile = inputFile; //outputFile and inputFile are global variables
	ifp = fopen(outputFile, "w");
	for (int i = 0; i < current_number_of_songs; ++i)
	{
		fprintf(ifp, "%s\n", music_library[i].title);
		fprintf(ifp, "%s\n", music_library[i].artist);
		fprintf(ifp, "%d\n", music_library[i].year_published);
	}
	fclose(ifp);
}
//-------------------------------------------------------------------------------------------------

//-----------------------------------------------------------------------------

void evaluate_command(char command)
{
	switch (command)
	{
	case 'I':
	case 'i':
		add_song_to_MusicLibrary();
		break;
	case 'P':
	case 'p':
		print_MusicLibrary();
		break;
	case 'D':
	case 'd':
		remove_song_from_MusicLibrary_by_name();
		break;
	case 'L':
	case 'l':
		load_desired_song();
		break;
	case 'Q':
	case 'q':
		store_MusicLibrary();
		break;
	}
}

//-----------------------------------------------READ_COMMAND-------------------------------------------------------
int check_if_valid_command(char command)
{
	if (command == 'I' || command == 'i' || command == 'P' || command == 'p')
		return 1;
	else if (command == 'D' || command == 'd' || command == 'L' || command == 'l' || command == 'Q' || command == 'q')
		return 1;
	else
		return 0;
}

char ask_for_input()
{
	char command;
	//scanf(" %c", &command);
	//printf("Enter a command ((I)nsert, (D)elete, (P)rint, (L)oad, or (Q)uit): ");
	command = getchar();
	return command;
}

char read_command()
{
	while (1)
	{
		char command = ask_for_input();
		if (check_if_valid_command(command) == 1)
			return command;
	}
}
//--------------------------------------------------------------------------------------------------------------------

//--------------------------------------------I/O FILE---------------------------------------------------------------

FILE *open_file(char *fileName, char fileMode[])
{
	FILE *ifp = fopen(fileName, fileMode);
	if (ifp == NULL)
	{
		fprintf(stderr, "FILE DOES NOT EXIST...CREATING FILE NOW... %s!\n", fileName);
		return NULL;
	}
	return ifp;
}
void read_file(FILE *file_input)
{
	struct Song track;
	while (fscanf(file_input, " %[^\n]", &(track.title)) == 1)
	{
		fscanf(file_input, " %[^\n]", &(track.artist));
		fscanf(file_input, " %d", &(track.year_published));
		add_song_to_array(track);
	}
}
//--------------------------------------------------------------------------------------------------------------------
void ask_for_command()
{
	while (1)
	{
		char command = read_command();
		evaluate_command(command);
		if (command == 'q' || command == 'Q')
			return;
	}
}

void load_file_and_ask_command(FILE *in_file)
{
	read_file(in_file);
	fclose(in_file);
	ask_for_command();
}

void open_and_load_default_file()
{
	inputFile = "myMusic";
	ifp = open_file(inputFile, "r");
	load_file_and_ask_command(ifp);
}

void file_given_check_existence_load(FILE *fileGiven)
{
	if (fileGiven == NULL)
		ask_for_command();

	else
		load_file_and_ask_command(fileGiven);
}

int main(int argc, char *argv[])
{
	if (argc == 1)
		open_and_load_default_file();

	else
	{
		inputFile = argv[1];
		ifp = open_file(inputFile, "r");
		file_given_check_existence_load(ifp);
	}
}
