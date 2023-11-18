#include <dirent.h> 
#include <stdio.h> 
#include <string.h>
#include <limits.h>

#define GREEN  "\x1B[32m"
#define BLUE  "\x1B[34m"

int first_depth;

int nesting_level(char *path) {
    int count = 0;
    int nesting_level = 0;
    while (*path != '\0') {
        if (*path == '/') {
            nesting_level += 1;
        }
        // moving pointer to the next character
        path++;
        count += 1;
    }
    return nesting_level;
}

void render(int item_type, int depth, char* item_name) {
  char horizontal[PATH_MAX] = "";
  int end = depth-first_depth;
  for (int i = 0; i <= end; i++) {
    if (i == end) {
      strcat(horizontal, item_name);
      break;
    }
    strcat(horizontal, "   ");
  }

  if (item_type == DT_DIR) {
    printf("%s%s\n", GREEN, horizontal);
  } else {
      printf("%s%s\n", BLUE, horizontal);
  }
}

void show_dir_content(char* path)
{
  DIR * d = opendir(path);
  if(d==NULL) return;
  struct dirent * dir;
  while ((dir = readdir(d)) != NULL)
    {
      if(dir-> d_type != DT_DIR) {
        char d_path[PATH_MAX];
        sprintf(d_path, "%s/%s", path, dir->d_name);
        render(dir -> d_type, nesting_level(d_path), dir->d_name);
      }
      if(dir -> d_type == DT_DIR && strcmp(dir->d_name,".")!=0 && strcmp(dir->d_name,"..")!=0 )
      {
        char d_path[PATH_MAX];
        sprintf(d_path, "%s/%s", path, dir->d_name);
        render(dir -> d_type, nesting_level(d_path), dir->d_name);
        show_dir_content(d_path);
      }
    }
    closedir(d);
}

int main()
{
  char cwd[PATH_MAX];

  if (getcwd(cwd, PATH_MAX) != NULL) {
      printf("Current working dir: %s\n", cwd);
      first_depth = nesting_level(cwd);
  } else {
      perror("getcwd() error");
      return 1;
  }

  show_dir_content(cwd);
  return(0);
}
