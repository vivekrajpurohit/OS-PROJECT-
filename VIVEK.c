/*
 * Problem Statement :-
 *      Sudesh Sharma is a linux expert who wants to have an online system where he can
 *      handle student queries. Since there can be multiple requests at any time he wishes
 *      to dedicate a fixed amount of time to every request so that everyone gets a fair
 *      share of his time. He will log into the System from 10am to 12am only. He wants to
 *      have separate requests queues for students and faculty. Implement a strategy for the
 *      same. The summary at the end of the session should include the total time he spent on
 *      handling queries and average query time.
 *
 * NAME  OF STUDENT 
 *      VIVEK RAJPUROHIT(11800965)
 *
 * Solution Implemented To Solve Problem:-
 *      Since we are planning to have different queues for students and faculties and each
 *      of them needs a fixed time quantum and in order to maximize the efficiency i will be
 *      calculating optimal time quantum to get the maximum efficiency in the shortest period
 *      of time.
 */

#include <stdio.h>
#include <limits.h>
#include <stdlib.h>
#include <windef.h>

//maxn means the maximum no of students and faculties we can have
#define maxn 100010

int time_quanta = 2, cur = 0;
typedef struct {
    char arrival_time[50];
    int pid, at, burst_time, priority, wt, id, td;
} data;
data faculty[maxn], student[maxn], q1[maxn], q2[maxn];

void calculate_time_in_seconds(data *d) {
    d->at = 0;
    int ghnta = 0, minutes = 0;
    for (int ide = 0; ide < 2; ide++)
        ghnta = ghnta * 10 + d->arrival_time[ide] - '0';

    for (int ide = 3; ide < 5; ide++)
        minutes = minutes * 10 + d->arrival_time[ide] - '0';

    d->at = ghnta * 60 + minutes;
}

int valid(char s[]) {
    for (int ide = 0; ide < 4; ide++) {
        if (ide == 2)continue;
        if (!isdigit(s[ide]))
            return 0;
    }
    int ghnta = 0, minutes = 0;
    for (int ide = 0; ide < 2; ide++)
        ghnta = ghnta * 10 + s[ide] - '0';

    for (int ide = 3; ide < 5; ide++)
        minutes = minutes * 10 + s[ide] - '0';

    if (ghnta < 10)
        return 0;
    return 1;
}

int comp(const void *appr, const void *bbbr) {
    data *A = (data *) appr;
    data *B = (data *) bbbr;
    if (A->at == B->at)
        return A->id - B->id;
    return A->at - B->at;
}

void solve_queries(int f, int s) {
    // Since we are not given whom to prioritize we will consider
    // prioritizing faculty first that means if we have anything in left
    // faculty in queue we process it first and then we
    // go to the children queue


    qsort((void *) faculty, f, sizeof(faculty[0]), comp);
    qsort((void *) student, s, sizeof(student[0]), comp);

    for (int ide = 0; ide < f; ide++)
        faculty[ide].id = ide;
    for (int ide = 0; ide < s; ide++)
        student[ide].id = ide;

    int min_arrival_time = 10 * 60;//10:00AM
    cur = min_arrival_time;
    int id1 = 0, id2 = 0, st1 = 0, st2 = 0;

    int x1 = 0, y1, x2 = 0, y2;
    while (id1 < f || id2 < s) {
        if (id1 < f)
            while (id1 < f && faculty[id1].at <= cur)
                q1[st1++] = faculty[id1++];

        if (id2 < s)
            while (id2 < s && student[id2].at <= cur)
                q2[st2++] = student[id2++];

        y1 = st1 - 1;
        y2 = st2 - 1;

        point1:
        while (x1 <= y1) {
            int delta = min(q1[x1].burst_time, time_quanta);
            q1[x1].burst_time -= delta;
            cur += delta;
            if (q1[x1].burst_time)
                q1[st1++] = q1[x1];
            else {
                int tp = q1[x1].id;
                faculty[tp].wt = cur - faculty[tp].at - faculty[tp].burst_time;
                faculty[tp].td = cur - faculty[tp].at;
            }

            if (id1 < f)
                while (id1 < f && faculty[id1].at <= cur)
                    q1[st1++] = faculty[id1++];

            if (id2 < s)
                while (id2 < s && student[id2].at <= cur)
                    q2[st2++] = student[id2++];
            y2 = st2 - 1;
            y1 = st1 - 1;
            x1++;
        }

        while (x2 <= y2) {
            int delta = min(q2[x2].burst_time, time_quanta);
            q2[x2].burst_time -= delta;
            cur += delta;

            if (q2[x2].burst_time)
                q2[st2++] = q2[x2];
            else {
                int tp = q2[x2].id;
                student[tp].wt = cur - student[tp].at - student[tp].burst_time;
                student[tp].td = cur - student[tp].at;
            }

            if (id1 < f)
                while (id1 < f && faculty[id1].at <= cur && faculty[id1].burst_time != 0)
                    q1[st1++] = faculty[id1++];

            if (id2 < s)
                while (id2 < s && student[id2].at <= cur && student[id1].burst_time != 0)
                    q2[st2++] = student[id2++];
            y2 = st2 - 1;
            y1 = st1 - 1;
            x2++;
            if (x1 <= y1)goto point1;
        }

        if (x1 <= y1 || x2 <= y2)
            continue;
        if (id1 < f && id2 < s)
            cur = min(faculty[id1].at, student[id2].at);
        else if (id1 < f)
            cur = faculty[id1].at;
        else if (id2 < s)
            cur = student[id2].at;
        else break;
    }
}

void solve() {
    int no_of_faculty, no_of_students;

    printf("WHAT IS NUMBER OF FACULTY:");
    scanf("%d", &no_of_faculty);

    printf("ENTER THE DETAILS OF FACULTY \n");
    printf("\n");
    for (int id = 0; id < no_of_faculty; id++) {
        //PID
        printf("WHAT IS PID OF FACULTY %d:", id + 1);
        scanf("%d", &faculty[id].pid);

//        Entering the arrival time
        printf("Time format :- (hh:mm) in 24 hr format\n");
        printf("WHAT IS ARRIVAL TIME:");
        scanf("%s", faculty[id].arrival_time);

        while (!valid(faculty[id].arrival_time)) {
            printf("Time format :- (hh:mm) in 24 hr format\n");
            printf("Enter correct arrival time");
            scanf("%s", faculty[id].arrival_time);
        }

        printf("Time should be in seconds\n");
        printf("Enter burst time:");
        scanf("%d", &faculty[id].burst_time);


        faculty[id].priority = 0;
        calculate_time_in_seconds(&faculty[id]);
        faculty[id].id = id;
        printf("\n");
    }

    printf("WHAT ARE TOTAL NUMBERS OD STUDENTS:");
    scanf("%d", &no_of_students);

    printf("Enter the details of each student\n");
    printf("\n");
    for (int id = 0; id < no_of_students; id++) {
        //PID
        printf("Enter PID of student %d:", id + 1);
        scanf("%d", &student[id].pid);

        //Entering the arrival time
        printf("Time format :- (hh::mm) in 24 hr format\n");
        printf("Enter arrival time:");
        scanf("%s", student[id].arrival_time);

        while (!valid(student[id].arrival_time)) {
            printf("Time format :- (hh::mm) in 24 hr format\n");
            printf("Enter correct arrival time");
            scanf("%s", student[id].arrival_time);
        }

        //Burst time
        printf("Time should be in seconds\n");
        printf("Enter burst time:");
        scanf("%d", &student[id].burst_time);

        student[id].priority = 1;
        calculate_time_in_seconds(&student[id]);
        faculty[id].id = id;

        printf("\n");
    }
    solve_queries(no_of_faculty, no_of_students);

    printf("Total summary at the end\n");
    int tp = -1;
    if (no_of_faculty)
        tp = faculty[0].at;
    if (no_of_students) {
        if (tp != -1) {
            tp = min(tp, student[0].at);
        } else {
            tp = student[0].at;
        }
    }
    if (tp == -1)
        tp = 0;

    printf("PID    WAITING TIME    TURNAROUND TIME    FACULTY\n");
    for (int i = 0; i < no_of_faculty; i++) {
        printf("%d      %d               %d                  True\n", faculty[i].pid, faculty[i].wt, faculty[i].td);
    }
    for (int i = 0; i < no_of_students; i++) {
        printf("%d      %d               %d                  False\n", student[i].pid, student[i].wt, student[i].td);
    }
    printf("Total time spent on handling queries :%d seconds\n", cur - tp);
    printf("Total average time spent on handling queries :%d seconds\n", (cur - tp) / (no_of_faculty + no_of_students));
}

//Write the input format in here to show the user how the input is handled
void input_format() {
    printf("Program to calculate Waiting time and Turn around time\n");
    printf("\n\n");
}

int main() {
    int test_case;
    input_format();
    printf("Enter total number of test cases:");
    scanf("%d", &test_case);

    while (test_case--)
        solve();

    return 0;
}
