/******************************************************************************
  A template program for developing a GAP solver. Subroutines to read instance
  data and compute the cost of a given solution are included.

  This program can also be used to compute the cost and check the feasibility
  of a solution given from a file. The format of a file is:
  for each job j from 1 to n in this order, the index of the agent (the value
  should be given as values from [1, m]) to which j is assigned. For example,
  if n=4 and m=3, and jobs 1, 2, 3 and 4 are assigned to agents 2, 1, 3 and 1,
  respectively, then the data in the file should be as follows:  2 1 3 1.

NOTE: Index i of agents ranges from 0 to m-1, and
index j of jobs   ranges from 0 to n-1 in the program,
while in the solution file,
index i of agents ranges from 1 to m, and
index j of jobs   ranges from 1 to n in the program.
Sorry for the confusion.

If you would like to use various parameters, it might be useful to modify
the definition of struct "Param" and mimic the way the default value of
"timelim" is given and how its value is input from the command line.
 ******************************************************************************/

#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <limits.h>
#include <math.h>
#include "cpu_time.c"

/***** default values of parameters ******************************************/
#define	TIMELIM	300	/* the time limit for the algorithm in seconds */
#define	GIVESOL	0	/* 1: input a solution; 0: do not give a solution */

typedef struct {
  int		timelim;	/* the time limit for the algorithm in secs. */
  int		givesol;	/* give a solution (1) or not (0) */
  /* Never modify the above two lines.  */
  /* You can add more components below. */
} Param;			/* parameters */

typedef struct {
  int	n;	/* number of jobs */
  int	m;	/* number of agents */
  int	**c;	/* cost matrix c_{ij} */
  int	**a;	/* resource requirement matrix a_{ij} */
  int	*b;	/* available amount b_i of resource for each agent i */
} GAPdata;	/* data of the generalized assignment problem */

typedef struct {
  double	timebrid;	/* the time before reading the instance data */
  double	starttime;	/* the time the search started */
  double	endtime;	/* the time the search ended */
  int		*bestsol;	/* the best solution found so far */
  /* Never modify the above four lines. */
  /* You can add more components below. */
} Vdata;		/* various data often necessary during the search */

/*************************** functions ***************************************/
void copy_parameters(int argc, char *arcv[], Param *param);
void read_instance(GAPdata *gapdata);
void prepare_memory(Vdata *vdata, GAPdata *gapdata);
void free_memory(Vdata *vdata, GAPdata *gapdata);
void read_sol(Vdata *vdata, GAPdata *gapdata);
void recompute_cost(Vdata *vdata, GAPdata *gapdata);
void *malloc_e(size_t size);

void random_init(int *sol, GAPdata *gapdata);
int calculate_cost(int *sol, GAPdata *gapdata);

/***** check the feasibility and recompute the cost **************************/
/***** NEVER MODIFY THIS SUBROUTINE! *****************************************/
void recompute_cost(Vdata *vdata, GAPdata *gapdata)
{
  int	i, j;		/* indices of agents and jobs */
  int	*rest_b;	/* the amount of resource available at each agent */
  int	cost, penal;	/* the cost; the penalty = the total capacity excess */
  int	temp;		/* temporary variable */

  rest_b = (int *) malloc_e(gapdata->m * sizeof(int));
  cost = penal = 0;
  for(i=0; i<gapdata->m; i++){rest_b[i] = gapdata->b[i];}
  for(j=0; j<gapdata->n; j++){
    rest_b[vdata->bestsol[j]] -= gapdata->a[vdata->bestsol[j]][j];
    cost += gapdata->c[vdata->bestsol[j]][j];
  }
  for(i=0; i<gapdata->m; i++){
    temp = rest_b[i];
    if(temp<0){penal -= temp;}
  }
  printf("recomputed cost = %d\n", cost);
  if(penal>0){
    printf("INFEASIBLE!!\n");
    printf(" resource left:");
    for(i=0; i<gapdata->m; i++){printf(" %3d", rest_b[i]);}
    printf("\n");
  }
  printf("time for the search:       %7.2f seconds\n",
      vdata->endtime - vdata->starttime);
  printf("time to read the instance: %7.2f seconds\n",
      vdata->starttime - vdata->timebrid);

  free((void *) rest_b);
}

/***** read a solution from STDIN ********************************************/
void read_sol(Vdata *vdata, GAPdata *gapdata)
{
  int	j;		/* index of jobs */
  int	value_read;	/* the value read by fscanf */
  FILE	*fp=stdin;	/* set fp to the standard input */

  for(j=0; j<gapdata->n; j++){
    fscanf(fp, "%d", &value_read);
    /* change the range of agents from [1, m] to [0, m-1] */
    vdata->bestsol[j] = value_read - 1;
  }
}

/***** prepare memory space **************************************************/
/***** Feel free to modify this subroutine. **********************************/
void prepare_memory(Vdata *vdata, GAPdata *gapdata)
{
  int j;

  vdata->bestsol = (int *)  malloc_e(gapdata->n * sizeof(int));
  /* the next line is just to avoid confusion */
  for(j=0; j<gapdata->n; j++){vdata->bestsol[j] = 0;}
}

/***** free memory space *****************************************************/
/***** Feel free to modify this subroutine. **********************************/
void free_memory(Vdata *vdata, GAPdata *gapdata)
{
  free((void *) vdata->bestsol);
  free((void *) gapdata->c[0]);
  free((void *) gapdata->c);
  free((void *) gapdata->a[0]);
  free((void *) gapdata->a);
  free((void *) gapdata->b);
}

/***** read the instance data ************************************************/
/***** NEVER MODIFY THIS SUBROUTINE! *****************************************/
void read_instance(GAPdata *gapdata)
{
  int	i, j;		/* indices of agents and jobs */
  int	value_read;	/* the value read by fscanf */
  FILE	*fp=stdin;	/* set fp to the standard input */

  /* read the number of agents and jobs */
  fscanf(fp, "%d", &value_read);	/* number of agents */
  gapdata->m = value_read;
  fscanf(fp,"%d",&value_read);		/* number of jobs */
  gapdata->n = value_read;

  /* initialize memory */
  gapdata->c    = (int **) malloc_e(gapdata->m * sizeof(int *));
  gapdata->c[0] = (int *)  malloc_e(gapdata->m * gapdata->n * sizeof(int));
  for(i=1; i<gapdata->m; i++){gapdata->c[i] = gapdata->c[i-1] + gapdata->n;}
  gapdata->a    = (int **) malloc_e(gapdata->m * sizeof(int *));
  gapdata->a[0] = (int *)  malloc_e(gapdata->m * gapdata->n * sizeof(int));
  for(i=1; i<gapdata->m; i++){gapdata->a[i] = gapdata->a[i-1] + gapdata->n;}
  gapdata->b    = (int *)  malloc_e(gapdata->m * sizeof(int));

  /* read the cost coefficients */   
  for(i=0; i<gapdata->m; i++){    
    for(j=0; j<gapdata->n; j++){
      fscanf(fp, "%d", &value_read);
      gapdata->c[i][j] = value_read;
    }
  }

  /* read the resource consumption */
  for(i=0; i<gapdata->m; i++){
    for(j=0; j<gapdata->n; j++){
      fscanf(fp, "%d", &value_read);
      gapdata->a[i][j] = value_read;
    }
  }

  /* read the resource capacity */
  for(i=0; i<gapdata->m; i++){    
    fscanf(fp,"%d", &value_read);
    gapdata->b[i] = value_read;
  }
}

/***** copy and read the parameters ******************************************/
/***** Feel free to modify this subroutine. **********************************/
void copy_parameters(int argc, char *argv[], Param *param)
{
  int i;

  /**** copy the parameters ****/
  param->timelim = TIMELIM;
  param->givesol = GIVESOL;
  /**** read the parameters ****/
  if(argc>0 && (argc % 2)==0){
    printf("USAGE: ./gap [param_name, param_value] [name, value]...\n");
    exit(EXIT_FAILURE);}
  else{
    for(i=1; i<argc; i+=2){
      if(strcmp(argv[i],"timelim")==0) param->timelim = atoi(argv[i+1]);
      if(strcmp(argv[i],"givesol")==0) param->givesol = atoi(argv[i+1]);
    }
  }
}

/***** malloc with error check ***********************************************/
void *malloc_e( size_t size ) {
  void *s;
  if ( (s=malloc(size)) == NULL ) {
    fprintf( stderr, "malloc : Not enough memory.\n" );
    exit( EXIT_FAILURE );
  }
  return s;
}

/***** subroutines ***********************************************/
void random_init(int *sol, GAPdata *gapdata) {
  int swap, tmp;
  bool is_feasible = true;
  int *rest_b = (int *) malloc_e(gapdata->m * sizeof(int));
  for (int i=0; i<gapdata->m; i++) rest_b[i] = gapdata->b[i];

  for (int i=0; i<gapdata->n; i++) {
    sol[i] = rand() % gapdata->m;

    rest_b[sol[i]] -= gapdata->a[sol[i]][i];
    if (rest_b[sol[i]] < 0) is_feasible = false;
  }

  while (!is_feasible) {
    swap = rand() % gapdata->m;
    for (int i=0; i<gapdata->n; i++) {
      tmp = sol[i];
      if (rest_b[tmp] > 0) continue;
      if (gapdata->a[tmp][i] > gapdata->a[swap][i] || rest_b[swap] > gapdata->a[tmp][i]) {
        sol[i] = swap;

        rest_b[tmp] += gapdata->a[tmp][i];
        rest_b[swap] -= gapdata->a[swap][i];
      }
    }

    is_feasible = true;
    for (int i=0; i<gapdata->m; i++) {
      if (rest_b[i] < 0) is_feasible = false;
    }
  }

  free((void *) rest_b);
}

int calculate_cost(int *sol, GAPdata *gapdata) {
  int cost = 0;
  for (int i=0; i<gapdata->n; i++) {
    cost += gapdata->c[sol[i]][i];
  }
  return cost;
}

/***** main ******************************************************************/
int main(int argc, char *argv[])
{
  Param		param;		/* parameters */
  GAPdata	gapdata;	/* GAP instance data */
  Vdata		vdata;		/* various data often needed during search */

  vdata.timebrid = cpu_time();
  copy_parameters(argc, argv, &param);
  read_instance(&gapdata);
  prepare_memory(&vdata, &gapdata);
  if(param.givesol==1){read_sol(&vdata, &gapdata);}
  vdata.starttime = cpu_time();

  /*
     Write your program here. Of course you can add your subroutines
     outside main(). At this point, the instance data is stored in "gapdata".
     gapdata.n	number of jobs n
     gapdata.m	number of agents m
     gapdata.c[i][j]	cost c_{ij} {i, j} = {agent, job} 
     gapdata.a[i][j]	resource requirement a_{ij} 
     gapdata.b[i]	available amount b_i of resource at agent i
     Note that i ranges from 0 to m-1, and j ranges from 0 to n-1. Note also
     that  you should write, e.g., "gapdata->c[i][j]" in your subroutines.
     Store your best solution in vdata.bestsol, then "recompute_cost" will
     compute its cost and its feasibility. The format of vdata.bestsol is:
     For each job j from 0 to n-1 in this order, the index of the agent 
     (the value should be given as values from [0, m-1]) to which j is
     assigned. For example, if n=4 and m=3, and jobs 0, 1, 2 and 3 are
     assigned to agents 1, 0, 2 and 0, respectively, then vdata.bestsol
     should be as follows:  
     vdata.bestsol[0] = 1
     vdata.bestsol[1] = 0
     vdata.bestsol[2] = 2
     vdata.bestsol[3] = 0.
     Note that you should write "vdata->bestsol[j]" in your subroutines.
     */

  const int INFEASIBLE_COST = 3;

  int a, b, tmp;

  int count = 0;
  int pre_val, new_val;
  int best_cost = INT_MAX;
  int impr;
  int impr_lim = gapdata.n * 5;

  int *new_bestsol = (int *) malloc_e(gapdata.n * sizeof(int));
  int *rest_b = (int *) malloc_e(gapdata.m * sizeof(int));

  int swap_cost, cur_cost;
  bool is_swap = false;

  while ((cpu_time() - vdata.starttime) < param.timelim) {
    count++;

    srand(count);
    random_init(new_bestsol, &gapdata);
    pre_val = calculate_cost(new_bestsol, &gapdata);
    impr = 0;

    for (int i=0; i<gapdata.m; i++) rest_b[i] = gapdata.b[i];
    for (int i=0; i<gapdata.n; i++) {
      rest_b[new_bestsol[i]] -= gapdata.a[new_bestsol[i]][i];
    }

    for (int i=0; i<gapdata.m; i++) {
      pre_val -= INFEASIBLE_COST * min(0, rest_b[i]);
    }
    new_val = pre_val;

    printf("INIT: %d\n", pre_val);

    while(impr < impr_lim) {
      a = rand() % gapdata.n;
      b = rand() % gapdata.n;
      if (a == b) continue;
      swap_cost
        = gapdata.c[new_bestsol[b]][a]
        + gapdata.c[new_bestsol[a]][b]
        + INFEASIBLE_COST
        * (max(0, gapdata.a[new_bestsol[b]][a] - rest_b[new_bestsol[b]])
            + max(0, gapdata.a[new_bestsol[a]][b] - rest_b[new_bestsol[a]]));

      cur_cost
        = gapdata.c[new_bestsol[b]][b]
        + gapdata.c[new_bestsol[a]][a]
        + INFEASIBLE_COST
        * (max(0, gapdata.a[new_bestsol[b]][b] - rest_b[new_bestsol[b]])
            + max(0, gapdata.a[new_bestsol[a]][a] - rest_b[new_bestsol[a]]));

      if (cur_cost > swap_cost) {
        tmp = new_bestsol[b];

        rest_b[tmp] += (gapdata.a[tmp][b] - gapdata.a[tmp][a]);
        rest_b[new_bestsol[a]] += (gapdata.a[new_bestsol[a]][a] - gapdata.a[new_bestsol[a]][b]);

        new_bestsol[b] = new_bestsol[a];
        new_bestsol[a] = tmp;
        is_swap = true;
      }

      if (is_swap) {
        new_val = 0;
        for (int j=0; j<gapdata.n; j++) {
          new_val += gapdata.c[new_bestsol[j]][j];
        }
        for (int i=0; i<gapdata.m; i++) {
          new_val -= INFEASIBLE_COST * min(0, rest_b[i]);
        }
        is_swap = false;
      }

      if (new_val >= pre_val) {
        impr++;
      } else {
        pre_val = new_val;
        impr = 0;
      }
    }

    if (new_val < best_cost && is_feasible(rest_b, &gapdata)) {
      for (int i=0; i<gapdata.n; i++) {
        vdata.bestsol[i] = new_bestsol[i];
      }
      best_cost = new_val;
    }

    printf("DONE Step: %d Cost: %d Time: %f\n", count, best_cost, (cpu_time() - vdata.starttime));
  }

  vdata.endtime = cpu_time();
  recompute_cost(&vdata, &gapdata);
  free_memory(&vdata, &gapdata);
  free((void *) rest_b);
  free((void *) new_bestsol);

  return EXIT_SUCCESS;
}
