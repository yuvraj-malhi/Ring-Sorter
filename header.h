#define MAX_DATA 256
#define NODE_PORT 5630

#define DATA_FILE "data.txt"
#define OUTPUT_FILE "output.txt"

typedef struct {
	int sndr_id;
	int rcvr_id;
	int num;			// number of integers in this message
	int data[MAX_DATA];
}Message;

void printMessage(Message mes){
	printf("\tsndr_id = %d", mes.sndr_id);
	printf("\trcvr_id = %d", mes.rcvr_id);
	printf("\tnum = %d", mes.num);

	printf("\tdata[%d] = %d\tdata[%d] = %d", 0, mes.data[0], mes.num-1, mes.data[mes.num-1]);

	printf("\n");
}

int compare_nums(int x, int y){
	// returns 1 if the required order of sorting is y then x, otherwise returns 0
	
	return y<x;		// ascending order
//	return x<y;	// descending order
}

void merge(int data[], int a, int b){
	int num = ((a+b)/2) - a + 1;
	int arr[num], brr[num];

	for(int i = 0; i<num; ++i){
		arr[i] = data[a+i];
		brr[i] = data[a+num+i];
	}
	
	int ind = a, li = 0, ri = 0;
	
	for(;li < num && ri < num;){
		if(compare_nums(arr[li], brr[ri])){
			data[ind] = brr[ri];
			++ri;
			++ind;
		}
		else{
			data[ind] = arr[li];
			++ind;
			++li;
		}
	}
	
	for(;li<num;++li)
		data[ind++] = arr[li];
		
	for(;ri<num;++ri)
		data[ind++] = brr[ri];
}

void custom_merge(int data[], int n){
	for(int i = 1; i<n; i=i*2 + 1){
		merge(data, 0, i);
	}
}

void printsent(int myID, int rcID, char*IP)
{
 	printf("NODE %-5d: SENT to\t(PORT   %-6d    IP %16s)  from  PORT %d\n", myID, NODE_PORT+1+rcID, IP, NODE_PORT+1+myID);

}

void printrcvd(int myID, int sdID, char*IP)
{
	printf("NODE %-5d: RCVD from\t(PORT   %-6d    IP %16s)   at   PORT %d\n", myID, NODE_PORT+1+myID, IP, NODE_PORT+1+sdID);

}

