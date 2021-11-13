#ifndef DATA_H
#define DATA_H

typedef struct _InjectData
{
    int process_id;
    char target_process[256];
    char dll_rel_path[256];
} InjectData;

#endif /* DATA_H */
