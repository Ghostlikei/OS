#include "../include/mytop.h"

// Helper functions
#ifndef __aarch64__
static inline int read_meminfo(imeminfo* info);
static inline void print_meminfo(imeminfo* info);
static inline int show_kinfo();
static inline int read_pinfo(int pid, iproc* procinfo);
#endif

/**
 * @note We need to do these following steps:
 * 1. read /proc/meminfo, [0]: pagesize, [1]: total, [2]: free, [3]: largest, [4]: cached
*/
int pmytop() {
#ifndef __aarch64__
    printf("MYTOP: print some sysinfo\n");
    // Get and calculate meminfo
    imeminfo meminfo;
    if (read_meminfo(&meminfo) < 0) {
        return -1;
    }

    print_meminfo(&meminfo);

    show_kinfo();

    // Get the process number and pid
    // Load them into nums
    DIR *dir;
    struct dirent *entry;
    int pids[kMaxProcess];
    int pcount = 0;

    dir = opendir("/proc");
    if (dir == NULL) {
        printf("Error: Could not open directory\n");
        exit(1);
    }

    while ((entry = readdir(dir)) != NULL) {
        // Check if entry name contains only digits
        int is_num = 1;
        for (int i = 0; entry->d_name[i] != '\0'; i++) {
            if (!isdigit((int) entry->d_name[i])) {
                is_num = 0;
                break;
            }
        }

        if (is_num) {
            pids[pcount++] = atoi(entry->d_name);
        }

        if (pcount > kMaxProcess) {
            break;
        }
    }
    closedir(dir);

    iproc procinfos[kMaxProcess];
    printf("------------mytop pidinfo------------\n");
    printf("PID USERNAME PRI NICE    SIZE STATE   TIME     CPU COMMAND\n");
    int total_ticks = 0;
    for (int i = 0; i < pcount; i++) {
        procinfos[i].pid = pids[i];
        read_pinfo(pids[i], &(procinfos[i]));
        total_ticks = total_ticks + (procinfos[i].ticks ? procinfos[i].ticks : 0);
    }

    for (int i = 0; i < (pcount > kMaxProcessTop ? kMaxProcessTop : pcount); i++) {
        printf("%3d %-8s %1d %4d    %6luK %7c %3lu      %2.2lf%% %s\n", 
                procinfos[i].pid,
                procinfos[i].username,
                procinfos[i].priority, 
                procinfos[i].nice, 
                procinfos[i].memory / 1024, 
                procinfos[i].state, 
                procinfos[i].ticks / 60, 
                procinfos[i].ticks / (double) total_ticks,
                procinfos[i].name);
    }

#endif
    return 0;
}

#ifndef __aarch64__
static inline int read_meminfo(imeminfo* info) {
    FILE* meminfo;

    unsigned long pagesize, total, free, largest, cached;
    meminfo = fopen("/proc/meminfo", "r");
    if (!meminfo) {
        // fprintf(stderr, "Mytop: cannot open meminfo\n");
        return -1;
    }

    if (fscanf(meminfo, "%lu %lu %lu %lu %lu", 
                &pagesize, &total, &free, &largest, &cached) < 5) {
        fprintf(stderr, "Mytop: cannot parse meminfo\n");
        return -1;
    }

    info->pagesize = pagesize;
    info->total = total;
    info->free = free;
    info->largest = largest;
    info->cached = cached;

    fclose(meminfo);
    return 0;
}

static inline void print_meminfo(imeminfo* info) {
    int mmemory = (info->pagesize * info->total) / 1024;
    int fmemory = (info->pagesize * info->free) / 1024;
    int cache_size = (info->pagesize * info->cached) / 1024;
    double cpu_used = 1 - (fmemory / (double) mmemory);

    printf("------------mytop meminfo------------\n");
    printf("Total memory(KB): %d\n", mmemory);
    printf("Free memory(KB: %d\n", fmemory);
    printf("Cache size(KB): %d\n", cache_size);
    printf("CPU used: %lf%% \n", cpu_used);
}

static inline int show_kinfo() {
    FILE* fp;
    if (!(fp = fopen("/proc/kinfo", "r"))) {
        fprintf(stderr, "Mytop: open kinfo error\n");
        return -1;
    }

    int proc_num, task_num;
    if (fscanf(fp, "%d %d", &proc_num, &task_num) < 2) {
        fprintf(stderr, "Mytop: parse kinfo error\n");
        return -1;
    }

    printf("------------mytop meminfo------------\n");
    printf("Total processes: %d, Total tasks: %d\n", proc_num, task_num);
    return 0;
}

static inline int read_pinfo(int pid, iproc* procinfo) {
    FILE* fp;
    char buf[kMaxTokenLen];
    sprintf(buf, "/proc/%d/psinfo", pid);

    fp = fopen(buf, "r");
    if (!fp) {
        // fprintf(stderr, "Mytop: open pid=%s file error\n", pid);
        return -1;
    }

    char name[512];
    if (fscanf(fp, "%*d %*c %*d %255s %c %*d %d %lu %*u %*u %*u %lu %*u %*u %*c %*d %*u %lu %*u %d",
        name,
        &(procinfo->state),
        &(procinfo->priority),
        &(procinfo->ticks),
        &(procinfo->memory),
        &(procinfo->uid),
        &(procinfo->nice)) < 7) {
    
        fprintf(stderr, "Mytop: Parse pid=%d file error\n", pid);
        return -1;
    }

    fclose(fp);
    strcpy(procinfo->name, name);
    strcpy(procinfo->username, getpwuid(procinfo->uid)->pw_name);
    return 0;
}
#endif