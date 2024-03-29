#define _XOPEN_SOUCE

#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <string.h>

#include "cd_data.h"

#define TMP_STRING_LEN 125

typedef enum {
    mo_invalid,
    mo_add_cat,
    mo_add_tracks,
    mo_del_cat,
    mo_find_cat,
    mo_list_cat_tracks,
    mo_del_tracks,
    mo_count_entries,
    mo_exit
} menu_options;

static int command_mode(int argc, char *argv[]);
static void announce(void);
static menu_options show_menu(const cdc_entry *current_cdc);
static int get_confirm(const char *question);
static int enter_new_cat_entry(cdc_entry *entry_to_update);
static void enter_new_track_entries(const cdc_entry *entry_to_add_to);
static void del_cat_entry(const cdc_entry *enrty_to_delete);
static void del_track_entries(const cdc_entry *enrty_to_delete);
static cdc_entry find_cat(void);
static void list_tracks(const cdc_entry *entry_to_use);
static void count_all_entries(void);
static void display_cdc(const cdc_entry *cdc_to_show);
static void display_cdt(const cdt_entry *cdt_to_show);
static void strip_return(char *string_to_strip);


void main(int argc, char *argv[])
{
    menu_options current_option;
    cdc_entry current_cdc_entry;
    int command_result;

    memset(&current_cdc_entry, '\0', sizeof(current_cdc_entry));

    if(argc > 1) {
        command_result = command_mode(argc, argv);
        exit(command_result);
    }

    announce();

    if(!database_initialize(0)) {
        fprintf(stderr, "Sorry, unable to initialize database");
        fprintf(stderr, "To create a new database use %s -i\n",argv[0]);
        exit(EXIT_FAILURE);
    }

    while(current_option != mo_exit) {
        current_option = show_menu(&current_cdc_entry);

        switch (current_option)
        {
            case mo_add_cat:
                if(enter_new_cat_entry(&current_cdc_entry)) {
                    if(!add_cdc_entry(current_cdc_entry)) {
                        fprintf(stderr, "Failed to add new entry\n");
                        memset(&current_cdc_entry, '\0',sizeof(current_cdc_entry));
                    }
                }
                break;
            case mo_add_tracks:
                enter_new_cat_entry(&current_cdc_entry);
                break;
            case mo_del_cat:
                del_cat_entry(&current_cdc_entry);
                break;
            case mo_find_cat:
                current_cdc_entry = find_cat();
                break;
            case mo_list_cat_tracks:
                list_tracks(&current_cdc_entry);
                break;
            case mo_del_tracks:
                del_track_entries(&current_cdc_entry);
                break;
            case mo_count_entries:
                count_all_entries();
                break;
            case mo_exit:
                break;
            case mo_invalid:
                break;
            default:
                break;
        }
    }

    database_close();
    exit(EXIT_SUCCESS);
}

static void announce(void)
{
    printf("\n\nWelcome to the demonstration CD catalog database \
                program\n");
}

/**
 * 展示菜单.
 */
static menu_options show_menu(const cdc_entry *cdc_selected)
{
    char tmp_str[TMP_STRING_LEN + 1];
    menu_options option_chosen = mo_invalid;

    while(option_chosen == mo_invalid) {
        if(cdc_selected->catalog[0]) {
            printf("\n\nCurrent entry:");
            printf("%s, %s, %s, %s\n", cdc_selected->catalog,
                cdc_selected->title,
                cdc_selected->type,
                cdc_selected->artist);
            
            printf("\n");
            printf("1 - add new CD\n");
            printf("2 - search for a CD\n");
            printf("3 - count the CDs and tarcks int database\n");
            printf("4 - re-enter tracks for current CD\n");
            printf("5 - delete this CD, and all its tracks\n");
            printf("6 - list tracks for this CD\n");
            printf("q - quit\n");
            printf("\n Option: ");
            fgets(tmp_str, TMP_STRING_LEN, stdin);

            switch (tmp_str[0]) {
                case '1': option_chosen = mo_add_cat;break;
                case '2': option_chosen = mo_find_cat;break;
                case '3': option_chosen = mo_count_entries;break;
                case '4': option_chosen = mo_add_tracks;break;
                case '5': option_chosen = mo_del_cat;break;
                case '6': option_chosen = mo_list_cat_tracks;break;
                case 'q': option_chosen = mo_exit;break;
            }
        }else{
            printf("\n\n");
            printf("1 - add new CD\n");
            printf("2 - search for a CD\n");
            printf("3 - count the CDs and tarcks int database\n");
            printf("q - quit\n");

            fgets(tmp_str, TMP_STRING_LEN, stdin);

            switch (tmp_str[0]) {
                case '1': option_chosen = mo_add_cat;break;
                case '2': option_chosen = mo_find_cat;break;
                case '3': option_chosen = mo_count_entries;break;
                case 'q': option_chosen = mo_exit;break;
            }
        }
    }
    return option_chosen;
}

/**
 * 确认输出框.
 */
static int get_confirm(const char *question)
{
    char tmp_str[TMP_STRING_LEN + 1];
    printf("%s", question);

    fgets(tmp_str, TMP_STRING_LEN, stdin);

    if(tmp_str[0] == 'Y' || tmp_str[0] == 'y'){
        return 1;
    }

    return 0;
}

/**
 * 输入一个新的CD.
 */
static int enter_new_cat_entry(cdc_entry *entry_to_update)
{
    cdc_entry new_entry;
    char tmp_str[TMP_STRING_LEN + 1];
    memset(&new_entry, '\0', sizeof(new_entry));

    printf("Enter catalog entry:");
    fgets(tmp_str, TMP_STRING_LEN, stdin);
    strip_return(tmp_str);
    strncpy(new_entry.catalog, tmp_str, CAT_CAT_LEN - 1);

    printf("Enter title: ");
    fgets(tmp_str, TMP_STRING_LEN, stdin);
    strip_return(tmp_str);
    strncpy(new_entry.title, tmp_str, CAT_TITLE_LEN - 1);

    printf("Enter type:");
    fgets(tmp_str, TMP_STRING_LEN, stdin);
    strip_return(tmp_str);
    strncpy(new_entry.type, tmp_str, CAT_TYPE_LEN - 1);

    printf("Enter artist:");
    fgets(tmp_str, TMP_STRING_LEN, stdin);
    strip_return(tmp_str);
    strncpy(new_entry.artist, tmp_str, CAT_TYPE_LEN - 1);

    printf("\nNew catalog entry is :-\n");
    display_cdc(&new_entry);

    if(get_confirm("Add this entry ?")) {
        memcpy(entry_to_update, &new_entry, sizeof(new_entry));
        return 1;
    }

    return 0;
}
/**
 * 添加一个新的曲目.
 */
static void enter_new_track_entries(const cdc_entry *entry_to_add_to)
{
    cdt_entry new_track, existing_track;
    char tmp_str[TMP_STRING_LEN + 1];
    int track_no = 1;

    if(entry_to_add_to->catalog[0] == '\0') return;

    printf("\n Updating tracks for %s\n", entry_to_add_to->catalog);
    printf("Press return to leave existing description unchanged,\n");
    printf(" a single d to delete this and remaining tracks,\n");
    printf(" or new track description\n");

    while(1) {
        memset(&new_track, '\0', sizeof(new_track));
        existing_track = get_cdt_entry(entry_to_add_to->catalog, track_no);

        if(existing_track.catalog[0]) {
            printf("\tTrack %d: %s\n", track_no, existing_track.track_txt);
            printf("\tNew text: ");
        }else{ 
            printf("\tTrack %d description: ", track_no);
        }

        fgets(tmp_str, TMP_STRING_LEN, stdin);
        strip_return(tmp_str);

        if(strlen(tmp_str) == 0) {
            if(existing_track.catalog[0] == '\0') {
                break;
            }else{
                track_no++;
                continue;
            }
        }

        if((strlen(tmp_str) == 1) && tmp_str[0] == 'd') {
            while(del_cdt_entry(entry_to_add_to->catalog, track_no)) {
                track_no++;
            }
            break;
        }

        strncpy(new_track.track_txt, tmp_str, TRACK_TTEXT_LEN - 1);
        strcpy(new_track.catalog, entry_to_add_to->catalog);
        new_track.track_no = track_no;

        if( !add_cdt_entry(new_track) ) {
            fprintf(stderr, "Failed to add new track\n");
            break;
        }

        track_no++;
    }
}
/**
 * 删除一个标题项
 */
static void del_cat_entry(const cdc_entry *enrty_to_delete)
{
    int track_no = 1;
    int delete_ok;

    display_cdc(enrty_to_delete);
    if (get_confirm("Delete this entry and all it's tracks?")) {
        do{
            delete_ok = del_cdt_entry(enrty_to_delete->catalog, track_no);
            track_no++;
        } while(delete_ok);

        if(!del_cdc_entry(enrty_to_delete->catalog)) {
            fprintf(stderr, "Failed to delete entry\n");
        }
    }
}

/**
 * 删除一个曲目项.
 */
static void del_track_entries(const cdc_entry *entry_to_delete)
{
    int track_no = 1;
    int delete_ok;

    display_cdc(entry_to_delete);
    if(get_confirm("Delete tracks for this entry?")) {
        do{
            delete_ok = del_cdt_entry(entry_to_delete->catalog, track_no);
            track_no++;
        }while(delete_ok);
    }
}

/**
 * 查找CD
 */
static cdc_entry find_cat(void) 
{
    cdc_entry item_found;
    char tmp_str[TMP_STRING_LEN + 1];

    int first_call = 1;
    int any_entry_found = 0;
    int string_ok;
    int entry_selected = 0;

    do{
        string_ok = 1;
        printf("Enter string to search for in catalog entry: ");
        fgets(tmp_str, TMP_STRING_LEN, stdin);
        strip_return(tmp_str);
        if(strlen(tmp_str) > CAT_CAT_LEN) {
            fprintf(stderr, "Sorry,string too long, maximum %d characters\n", CAT_CAT_LEN);
            string_ok = 0;
        }
    }while(!string_ok);

    while(!entry_selected) {
        item_found = search_cdc_entry(tmp_str, &first_call);
        if(item_found.catalog[0] != '\0') {
            any_entry_found = 1;
            printf("\n");
            display_cdc(&item_found);
            if(get_confirm("This entry?")) {
                entry_selected = 1;
            }
        }else{
            if(any_entry_found) printf("Sorry, no more matches found\n");
            else printf("Sorry, nothing found\n");
            break;
        }
    }

    return item_found;
}

/**
 * 列出CD中的曲目.
 */
static void list_tracks(const cdc_entry *entry_to_use)
{
    int track_no = 1;
    cdt_entry entry_found;

    display_cdc(entry_to_use);
    printf("\nTracks\n");
    do{
        entry_found = get_cdt_entry(entry_to_use->catalog, track_no);
        if(entry_found.catalog[0]) {
            display_cdt(&entry_found);
            track_no++;
        }
    }while(entry_found.catalog[0]);

    get_confirm("Press enter");
}

/**
 * 统计所有的cd.
 */
static void count_all_entries(void)
{
    int cd_entries_found = 0;
    int track_entries_found = 0;
    cdc_entry cdc_found;
    cdt_entry cdt_found;
    int track_no = 1;
    int first_time = 1;

    char *search_string = "";

    do{
        cdc_found = search_cdc_entry(search_string, &first_time);
        if(cdc_found.catalog[0]) {
            cd_entries_found++;
            track_no = 1;
            do{
                cdt_found = get_cdt_entry(cdc_found.catalog, track_no);
                if(cdt_found.catalog[0]) {
                    track_entries_found++;
                    track_no++;
                }
            }while(cdt_found.catalog[0]);
        }
    }while(cdc_found.catalog[0]);

    printf("Found %d CDs, with a total of %d tracks\n", cd_entries_found, track_entries_found);

    get_confirm("Press return");
}

/**
 * 展示一个标题项.
 */
static void display_cdc(const cdc_entry *cdc_to_show)
{
    printf("Catalog: %s\n", cdc_to_show->catalog);
    printf("\ttitle: %s\n", cdc_to_show->title);
    printf("\ttype:  %s\n", cdc_to_show->type);
    printf("\tartist: %s\n", cdc_to_show->artist);
}

/**
 * 展示所有的曲目.
 */
static void display_cdt(const cdt_entry *cdt_to_show)
{
    printf("%d: %s\n", cdt_to_show->track_no, cdt_to_show->track_txt);
}

/**
 * 跳过.
 */
static void strip_return(char *string_to_strip)
{
    int len;

    len = strlen(string_to_strip);
    if(string_to_strip[len - 1] == '\n') string_to_strip[len - 1] == '\0';
}

/**
 * 获取模式.
 */
static int command_mode(int argc, char *argv[]) 
{
    int c;
    int result = EXIT_SUCCESS;

    char *prog_name = argv[0];

    extern char *optarg;
    extern int optind, opterr, optopt;

    while((c = getopt(argc, argv, ":i")) != -1) {
        switch(c) {
            case 'i' :
                if(!database_initialize(1)) {
                    result = EXIT_FAILURE;
                    fprintf(stderr, "Failed to initialize databse\n");
                }
                break;
            case ':':
            case '?':
            default:
                fprintf(stderr, "Usage: %s [-i]\n", prog_name);
                result = EXIT_FAILURE;
                break;
        }
    }

    return result;
}