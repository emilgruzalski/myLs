#include <stdio.h>
#include <stdlib.h>
#include <dirent.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <unistd.h>
#include <fcntl.h>
#include <time.h>
#include <pwd.h>
#include <grp.h>
#include <string.h>

/* funkcja odpowiedzialna za transformacje miesiecy 0-11 na char (nazwy polskie) */ 
void showtime(struct tm *time)
{
    char *mon;
    switch(time->tm_mon)
    {

        case 0:
        {
            mon = "styczen";
            printf("%11s", mon);
            break;
        }
        case 1:
        {
            mon = "luty";
            printf("%11s", mon);
            break;
        }
        case 2:
        {
            mon = "marzec";
            printf("%11s", mon);
            break;
        }
        case 3:
        {
            mon = "kwiecien";
            printf("%11s", mon);
            break;
        }
        case 4:
        {
            mon = "maj";
            printf("%11s", mon);
            break;
        }
        case 5:
        {
            mon = "czerwiec";
            printf("%11s", mon);
            break;
        }
        case 6:
        {
            mon = "lipiec";
            printf("%11s", mon);
            break;
        }
        case 7:
        {
            mon = "sierpien";
            printf("%11s", mon);
            break;
        }
        case 8:
        {
            mon = "wrzesien";
            printf("%11s", mon);
            break;
        }
        case 9:
        {
            mon = "pazdziernik";
            printf("%11s", mon);
            break;
        }
        case 10:
        {
            mon = "listopad";
            printf("%11s", mon);
            break;
        }
        case 11:
        {
            mon = "grudzien";
            printf("%11s", mon);
            break;
        }
    }
}

int main(int argc, char *argv[])
{    
/* tryb pierwszy - listowanie ls -la */    
    if (argc == 1)
    {
        struct dirent *direntp; /* nazwa pliku */
        struct stat fileStat; /* statystyki np. prawa dostepu */
        struct tm *time; /* rozbijanie czasu na pomniejsze element np. dni */
        struct passwd *pwd; /* wyswietlanie wlasciciela pliku */
        struct group *grp; /* wyswietlanie grupy pliku */
        DIR *dirp = opendir("."); /* otworzenie aktualnego folderu */
        for(;;)
        {
            direntp = readdir(dirp);
            lstat(direntp->d_name, &fileStat); /* pobieranie danych z aktualnie otworzonego pliku */
            if(direntp == NULL) break;
        /* sprawdzanie jaki to typ pliku - zwykly, folder, symlink */
            if (S_ISREG(fileStat.st_mode))
            {
                printf("-");
            } else if (S_ISDIR(fileStat.st_mode))
            {
                printf("d");
            } else if (S_ISLNK(fileStat.st_mode))
            {
                printf("l");
            }
        /* wyswietlanie praw dostepu */
            printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-"); 
            printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-"); 
            printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-"); 
            printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-"); 
            printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-"); 
            printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-"); 
            printf( (fileStat.st_mode & S_IROTH) ? "r" : "-"); 
            printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-"); 
            printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
            time = gmtime(&fileStat.st_mtime); /* pobranie czasu ostatniej  modyfikacji */
            pwd = getpwuid(fileStat.st_uid); /* pobranie nazwy wlasciciela pliku */
            grp = getgrgid(fileStat.st_gid); /* pobranie nazwy grupy pliku */
        /* specialny przypadek z plikiem ".." (komenda stat wyjasnia +1 +2 +2) */
        /*    if (strcmp((direntp->d_name), "..") == 0) 
            {
                printf("  %lu %s  %s  %ld %02i-%02i %02i:%02i %s\n", fileStat.st_nlink, pwd->pw_name, grp->gr_name, fileStat.st_size,
            (time->tm_mon +1), time->tm_mday, (time->tm_hour +2), time->tm_min, direntp->d_name); */
        /* funkcja wypisujaca realna droge do pliku z linku */
        /*    } else */ if (S_ISLNK(fileStat.st_mode)) {
            char *path;
            path = realpath(direntp->d_name, NULL);
            printf("  %lu %s  %s  %ld %02i-%02i %02i:%02i %s -> %s\n", fileStat.st_nlink, pwd->pw_name, grp->gr_name, fileStat.st_size,
            (time->tm_mon +1), time->tm_mday, (time->tm_hour +1), time->tm_min, direntp->d_name, path);
        /* sekwencja wypisujaca rzeczy do trybu pierwszego */
            } else {
                printf("  %lu %s  %s  %ld %02i-%02i %02i:%02i %s\n", fileStat.st_nlink, pwd->pw_name, grp->gr_name, fileStat.st_size,
            (time->tm_mon +1), time->tm_mday, (time->tm_hour +1), time->tm_min, direntp->d_name);
            }
        }
        closedir(dirp);
    /* tryb drugi - komenda stat */
    } else if (argc == 2)
    {
        struct stat fileStat; 
        struct tm *time;
        char *path; /* zmienna potrzebna do funkcji realpath (buffor) */
        lstat(argv[1], &fileStat);
        printf("Informacje o %s:\n", argv[1]);
        printf("Typ pliku:   ");
    /* sprawdzanie jaki to plik */
        if (S_ISREG(fileStat.st_mode))
        {
            printf("zwykly plik\n");
        } else if (S_ISDIR(fileStat.st_mode))
        {
            printf("katalog\n");
        } else if (S_ISLNK(fileStat.st_mode))
        {
            printf("link symboliczny\n");
        }
    /* rozwidlenie na sciezke oraz sciezke i wskazuje na */
        if (S_ISLNK(fileStat.st_mode))
        {   
            char ptr[100000];
            getcwd(ptr, sizeof(ptr));
            printf("Sciezka:     %s/%s\n", ptr, argv[1]); 
            path = realpath(argv[1],NULL);
            printf("Wskazuje na: %s\n", path);
        } else 
        {
            path = realpath(argv[1],NULL);
            printf("Sciezka:     %s\n", path); 
        }
    /* "formatowanie" odmiany slowa bajt */
        if ((fileStat.st_size == 0 || fileStat.st_size > 4) == 1)
        {
            printf("Rozmiar:     %ld bajtow\n", fileStat.st_size);
        } else if (fileStat.st_size == 1)
        {
            printf("Rozmiar:     %ld bajt\n", fileStat.st_size);
        } else if ((fileStat.st_size > 1 && fileStat.st_size < 5) == 1)
        {
            printf("Rozmiar:     %ld bajty\n", fileStat.st_size);
        }
    /* wypisanie uprawnien */
        printf("Uprawnienia: ");
        printf( (fileStat.st_mode & S_IRUSR) ? "r" : "-"); 
        printf( (fileStat.st_mode & S_IWUSR) ? "w" : "-"); 
        printf( (fileStat.st_mode & S_IXUSR) ? "x" : "-"); 
        printf( (fileStat.st_mode & S_IRGRP) ? "r" : "-"); 
        printf( (fileStat.st_mode & S_IWGRP) ? "w" : "-"); 
        printf( (fileStat.st_mode & S_IXGRP) ? "x" : "-"); 
        printf( (fileStat.st_mode & S_IROTH) ? "r" : "-"); 
        printf( (fileStat.st_mode & S_IWOTH) ? "w" : "-"); 
        printf( (fileStat.st_mode & S_IXOTH) ? "x" : "-");
        printf("\n");
    /* wypisanie odpowiedniego czasu: atime - uzycie, mtime - modyfikacja, ctime - zmiany stanu */ 
        time = gmtime(&fileStat.st_atime);
        printf("Ostatnio uzywany:        %2i ",time->tm_mday);
        showtime(time);
        printf(" %4i roku o %02i:%02i:%02i\n",(time->tm_year +1900), (time->tm_hour +1), time->tm_min, time->tm_sec);
    /* wyjatek dla ".." */
    /*    if (strcmp((argv[1]), "..") == 0)
        {
        time = gmtime(&fileStat.st_mtime);
        printf("Ostatnio modyfikowany:   %2i ", time->tm_mday);
        showtime(time);
        printf(" %4i roku o %02i:%02i:%02i\n",(time->tm_year +1900), (time->tm_hour +2), time->tm_min, time->tm_sec);
        time = gmtime(&fileStat.st_ctime);
        printf("Ostatnio zmieniany stan: %2i ", time->tm_mday);
        showtime(time);
        printf(" %4i roku o %02i:%02i:%02i\n",(time->tm_year +1900), (time->tm_hour +2), time->tm_min, time->tm_sec);
        } else 
        { */
        time = gmtime(&fileStat.st_mtime);
        printf("Ostatnio modyfikowany:   %2i ", time->tm_mday);
        showtime(time);
        printf(" %4i roku o %02i:%02i:%02i\n",(time->tm_year +1900), (time->tm_hour +1), time->tm_min, time->tm_sec);
        time = gmtime(&fileStat.st_ctime);
        printf("Ostatnio zmieniany stan: %2i ", time->tm_mday);
        showtime(time);
        printf(" %4i roku o %02i:%02i:%02i\n",(time->tm_year +1900), (time->tm_hour +1), time->tm_min, time->tm_sec);
    /*    } */
    }
    return 0;
}