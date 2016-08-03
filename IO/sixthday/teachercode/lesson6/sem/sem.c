#include <head.h>

union semun {
	int		val;	/* Value for SETVAL */
	struct semid_ds *buf;	/* Buffer for IPC_STAT, IPC_SET */
	unsigned short  *array;	/* Array for GETALL, SETALL */
	struct seminfo  *__buf;	/* Buffer for IPC_INFO
							   (Linux-specific) */
};

void init_sem_value(int sem_id,int sem_num,int value)
{
	union semun sem_val;
	
	sem_val.val = value;
	
	if(semctl(sem_id,sem_num,SETVAL,sem_val) < 0)
	{
		perror("Fail to semctl");
		exit(EXIT_FAILURE);
	}

	return ;
}

int init_sems(const char *filename,int nsem,int *pcreate_flag)
{
	key_t key;
	int sem_id;

	key  = ftok(filename,'b');
	if(key < 0){
		perror("Fail to ftok");
		exit(EXIT_FAILURE);
	}
	
	sem_id = semget(key,nsem,IPC_CREAT|IPC_EXCL| 0666);
	if(sem_id < 0)
	{
		//已经存在
		if(errno == EEXIST)
		{
			*pcreate_flag = 0;
			//重新获得ID
			sem_id = semget(key,nsem,IPC_CREAT | 0666);
		
		}else{
			//其他错误
			perror("Fail to semget");
			exit(EXIT_FAILURE);
		}
		
	//不存在，创建好后，初始化
	}else{
		*pcreate_flag = 1;	
		init_sem_value(sem_id,0,0);
		init_sem_value(sem_id,1,1);
	}

	return sem_id;
}

void P(int sem_id,int sem_num)
{
	struct sembuf sem;

	sem.sem_num = sem_num;
	sem.sem_op  = -1;
	sem.sem_flg = 0;
	
	if(semop(sem_id,&sem,1) < 0)
	{
		perror("Fail to semop for P\n");
		exit(EXIT_FAILURE);
	}

	return ;
}

void V(int sem_id,int sem_num)
{
	struct sembuf sem;

	sem.sem_num = sem_num;
	sem.sem_op  = 1;
	sem.sem_flg = 0;
	
	if(semop(sem_id,&sem,1) < 0)
	{
		perror("Fail to semop for V\n");
		exit(EXIT_FAILURE);
	}

	return ;
}

void delete_sems(int sem_id)
{
	int ret;

	ret = semctl(sem_id,0,IPC_RMID);
	if(ret < 0){
		perror("Fail to semctl for delete");	
		exit(EXIT_FAILURE);
	}

	return;
}
