#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
	
/// @brief 
typedef int errno_t;
int G, M, k = 1, stop = 2;
double threshold_value;
char attribute[40][30], object[100][100];

typedef struct node 
{
    int Concept_no;
    int index;
    int val;
    struct node *next;
}node_C; // LIST TO STORE CONCEPTS OBJECTS AND ATTRIBUTES.
node_C *Obj_list = NULL;
node_C *Attr_list = NULL;
node_C *Obj_head_ref = NULL;
node_C *Attr_head_ref = NULL;
int IND_A = 0;
int IND_O = 0;

typedef struct node1
{
    int obj_first;
    int obj_last;
    int attr_first;
    int attr_last;
    struct node1 *next;
} FL_INDEX; // LIST TO STORE CONCEPTS FTRST AND LAST INDEX.
FL_INDEX *Index = NULL;
FL_INDEX *Index_head = NULL;

// FUNCTIONS FOR QUICK SORT
void swap(int *a, int *b)
{
    int t = *a;
    *a = *b;
    *b = t;
}
int partition(int arr[], int low, int high)
{
    int pivot = arr[high];
    int i = (low - 1);
    for (int j = low; j <= high - 1; j++)
    {
        if (arr[j] < pivot)
        {
            i++;
            swap(&arr[i], &arr[j]);
        }
    }
    swap(&arr[i + 1], &arr[high]);
    return (i + 1);
}
void quickSort(int arr[], int low, int high)
{
    if (low < high)
    {
        int pi = partition(arr, low, high);
        quickSort(arr, low, pi - 1);
        quickSort(arr, pi + 1, high);
    }
}

void derivative(int *A, int g_derivative[], int Context[G][M]) // single closure
{
    for (int i = 0; i < M; i++)
    {
        if (*(A + i) == 0)
            continue;
        for (int j = 0; j < G; j++)
        {
            if (Context[j][i] == 0)
                g_derivative[j] = 0;
        }
    }
}

void initialize(int A[], int n, int value)
{
    for (int i = 0; i < n; i++)
        A[i] = value;
    return;
}
void initialize_closure(int *A, int n, int value)
{
    for (int i = 0; i < n; i++)
        *(A + i) = value;
    return;
}

int power(int a, int b)
{
    int temp = 1;
    for (int i = 0; i < b; i++)
        temp *= a;
    return temp;
}

void binary(int n, int binary_array[], int number_of_attributes)
{
    int i = 0;
    for (int i = 0; i < number_of_attributes; i++)
    {
        binary_array[i] = n % 2;
        n = n / 2;
    }
}

double stability(int *A, int B[], int Context[G][M])
{
    double num = 0, dem = 0;
    int number_of_attributes = 0;
    for (int i = 0; i < M; i++)
    {
        if (*(A + i) == 1)
            number_of_attributes++;
    }
    if (number_of_attributes == 0)
        return 1;
    int temp[number_of_attributes], subset[M], p = 0;
    for (int i = 0; i < M; i++)
    {
        if (*(A + i) == 1)
        {
            temp[p] = i;
            p++;
        }
    }
    // for(int i=0;i<number_of_attributes;i++)
    dem = power(2, number_of_attributes);
    for (int i = 0; i < dem; i++)
    {
        initialize(subset, M, 0);
        int binary_array[number_of_attributes];
        binary(i, binary_array, number_of_attributes);
        for (int j = 0; j < number_of_attributes; j++)
        {
            if (binary_array[j] == 1)
                subset[temp[j]] = 1;
        }
        int *array_pointer = subset;
        int subset_derivative[G];
        initialize(subset_derivative, G, 1);
        derivative(array_pointer, subset_derivative, Context);
        int flag = 0;
        for (int j = 0; j < G; j++)
        {
            if (subset_derivative[j] != B[j])
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
            num++;
    }
    return num / dem;
}

int *Closure(int Context[G][M], int *A)
{

    int *B = (int *)calloc(M, sizeof(int));
    initialize(B, M, 1);

    for (int i = 0; i < G; i++)
    {
        int flag = 0;
        for (int j = 0; j < M; j++)
        {
            if (*(A + j) == 1 && Context[i][j] != 1)
            {
                flag = 1;
                break;
            }
        }
        if (flag == 0)
        {
            for (int j = 0; j < M; j++)
            {
                if (B[j] == 1 && Context[i][j] == 1)
                    continue;
                else
                    B[j] = 0;
            }
        }
    }
    return B;
}
void print_attribute(int index)
{
    for (int i = 0; i < M; i++)
    {
        if (attribute[index][i] == '\0')
            return;
        printf("%c", attribute[index][i]);
    }
}
void print_object(int index)
{
    for (int i = 0; i < G; i++)
    {
        if (object[index][i] == '\0')
            return;
        printf("%c", object[index][i]);
    }
}
void Print(int *a, int n, int Context[G][M])
{
    int g_derivative[G], num_attribute = 0;
    initialize(g_derivative, G, 1);
    derivative(a, g_derivative, Context);
    double s = stability(a, g_derivative, Context);
    if (s < threshold_value)
        return;

    printf("Concept %d: { ", k);
    Index->obj_first = IND_O; // first index of concept objects.
    int checkFieObjSet = 0;
    for (int i = 0; i < G; i++)
    {

        if (g_derivative[i] == 1)
        {
            Obj_list->Concept_no = k;
            Obj_list->index = IND_O; // Object index
            Obj_list->val = i + 1;
            node_C *newObjNode = (node_C *)malloc(100000);
            Obj_list->next = newObjNode;
            Obj_list = newObjNode;
            Obj_list->next = NULL;
            print_object(i);
            IND_O++;
        }
        else
            checkFieObjSet++;
    }
    if (checkFieObjSet == G)
    {
        Obj_list->Concept_no = k;
        Obj_list->index = IND_O;
        Obj_list->val = 0;
        node_C *newObjNode = (node_C *)malloc(100000);
        Obj_list->next = newObjNode;
        Obj_list = newObjNode;
        Obj_list->next = NULL;
        IND_O++;
    }
    Index->obj_last = IND_O - 1; // last index of concept objects.
    printf("} -> { ");
    Index->attr_first = IND_A; // first index of concept attribute.
    int checkFiObjSet = 0;
    for (int i = 0; i < n; i++)
    {
        if (*(a + i) == 1)
        {
            num_attribute++;
            Attr_list->Concept_no = k;
            Attr_list->index = IND_A; // attribute index
            Attr_list->val = i + 1;
            node_C *newAttrNode = (node_C *)malloc(100000);
            Attr_list->next = newAttrNode;
            Attr_list = newAttrNode;
            Attr_list->next = NULL;
            print_attribute(i + 1);
            IND_A++;
        }
        else
            checkFiObjSet++;
    }
    printf("}");
    if (checkFiObjSet == n)
    {
        Attr_list->Concept_no = k;
        Attr_list->index = IND_A;
        Attr_list->val = 0;
        node_C *newAttrNode = (node_C *)malloc(100000);
        Attr_list->next = newAttrNode;
        Attr_list = newAttrNode;
        Attr_list->next = NULL;
        IND_A++;
    }
    Index->attr_last = IND_A - 1; // last index of concept attribute.
    FL_INDEX *newIndexNode = (FL_INDEX *)malloc(100000);
    Index->next = newIndexNode;
    Index = Index->next;
    Index->next = NULL;
    k++;
    if (num_attribute == M)
    {
        printf("\nStability = 1.0000\n");
        printf("\n");
        return;
    }
    printf("\nStability = %0.4f\n", s);
    printf("\n");
}
int *NextClosure(int *A, int Context[G][M])
{
    for (int m = M - 1; m >= 0; m--)
    {
        if (*(A + m) == 1)
            *(A + m) = 0;
        else
        {
            int temp[M];
            for (int i = 0; i < M; i++)
            {
                if (*(A + i) == 0)
                    temp[i] = 0;
                else
                    temp[i] = 1;
            }
            temp[m] = 1;
            int *B = temp;
            B = Closure(Context, B);
            int flag = 0;
            for (int i = m - 1; i >= 0; i--)
            {
                if (*(B + i) == 1 && *(A + i) == 0)
                {
                    flag = 1;
                    break;
                }
            }
            if (flag == 0)
            {
                return B;
            }
        }
    }
    *A = stop;
    return A;
}
void Concepts(int Context[G][M])
{
    int A[M];
    initialize(A, M, 0);
    int *A_Closure = A;
    A_Closure = Closure(Context, A_Closure);
    do
    {
        Print(A_Closure, M, Context);
        A_Closure = NextClosure(A_Closure, Context);
    } while (*A_Closure != stop);
}
int main()
{

    Obj_list = (node_C *)malloc(100000000);
    Attr_list = (node_C *)malloc(100000000);
    Obj_head_ref = Obj_list;
    Attr_head_ref = Attr_list;
    G = 50;
    M = 39;
    Index = (FL_INDEX *)malloc(100000000);
    Index_head = Index; // first node of concept object_index and attribute_index.

    printf("Enter the threshold value between 0 to 1: ");
    scanf("%lf", &threshold_value);
    if (threshold_value < 0 || threshold_value > 1)
    {
        printf("Wrong threshold value.");
        return 0;
    }

    FILE *fp;
    errno_t err;
    // Please enter the file path according to your system below.
    err = fopen_s(&fp, "C:\\Users\\DELL\\OneDrive\\Desktop\\Research\\FCA_Scaled1.txt", "r");
    char c;
    int attribute_flag = 1, space_count = 0, attribute_number = 0, attribute_size = 0;
    int object_flag = 1, object_number = 0, object_size = 0;
    int row = 0, column = 0, Context[50][39];
    while ((c = fgetc(fp)) != EOF)
    {
        if (attribute_flag) // attributes read
        {
            if (c == '\n')
            {
                attribute_flag = 0;
                continue;
            }
            if (c == ' ')
            {
                if (attribute_size == 0)
                    continue;
                space_count++;
            }
            if (space_count >5 || attribute_size > 28)
            {
                attribute[attribute_number][attribute_size] = '\0';
                space_count = 0;
                attribute_number++;
                attribute_size = 0;
                continue;
            }
            attribute[attribute_number][attribute_size] = c;
            attribute_size++;
        }
        else // object and data read code
        {
            if (c == '\n')
            {
                object_flag = 1;
                continue;
            }
            if (object_flag && (c == '0' || c == '1'))
            {
                object_flag = 0;
                object[object_number][object_size] = '\0';
                object_number++;
                object_size = 0;
            }
            if (object_flag)
            {
                object[object_number][object_size] = c;
                object_size++;
            }
            else
            {
                if (c == '1' || c == '0')
                {
                    Context[row][column] = c - 48;
                    column++;
                    if (column == 39)
                    {
                        row++;
                        column = 0;
                    }
                }
            }
        }
    }
    fclose(fp);
    printf("A - Attribute\n");
    printf("T - Object\n");
    Concepts(Context);

    int N;
    printf("Enter number of Objects: ");
    scanf("%d", &N);
    int SearchObjSet[N];
    printf("\nEnter Object Set to be searched : ");
    for (int i = 0; i < N; i++)
    {
        scanf("%d", &SearchObjSet[i]);
        print_object(SearchObjSet[i] - 1);
    }
    if (N == 0 && SearchObjSet[0] == 0)
    {
        printf("\nCorresponding Attribute set to the given Object Set are : ");
        for (int i = 1; i <= M; i++)
            printf("%d ", i);
    }
    else
    {
        quickSort(SearchObjSet, 0, N - 1);
        // check if given object set is present.
        node_C *Traverse_Obj_list = Obj_head_ref;
        int c = 0, F, z, a = 0;
        z = Traverse_Obj_list->Concept_no; // stores index of previous object
        while (Traverse_Obj_list != NULL)
        {
            node_C *T = Traverse_Obj_list;
            F = T->index;
            int i, x, y;
            for (i = 0; i < N; i++)
            {
                if (T->val != SearchObjSet[i])
                    break;
                x = T->Concept_no; // stores index of current object.
                T = T->next;
                y = T->Concept_no; // stores index of next object.
            }
            if (i == N && x != y && (x != z || a == 0))
            {
                c++;
                // printf("FOUND AT %d",F);
                break;
            }
            a = 1;
            z = Traverse_Obj_list->Concept_no; // stores index of previous object
            Traverse_Obj_list = Traverse_Obj_list->next;
        }
        if (c == 0)
            printf("NOT FOUND ");
        else
        {
            int FirstAttrInd, LastAttrInd;
            FL_INDEX *TraverseIndexList = Index_head;
            while (TraverseIndexList != NULL)
            {
                if (TraverseIndexList->obj_first == F)
                {
                    FirstAttrInd = TraverseIndexList->attr_first;
                    LastAttrInd = TraverseIndexList->attr_last;
                    break;
                }
                TraverseIndexList = TraverseIndexList->next;
            }
            // printf("\n%d  %d",FirstAttrInd,LastAttrInd);
            printf("\nCorresponding Attribute set to the given Object Set are : ");
            node_C *Traverse_Attr_list = Attr_head_ref;
            while (Traverse_Attr_list != NULL)
            {
                if (Traverse_Attr_list->index >= FirstAttrInd && Traverse_Attr_list->index <= LastAttrInd)
                    print_attribute((Traverse_Attr_list->val));
                    //printf("%d \n",Traverse_Attr_list->val);
                Traverse_Attr_list = Traverse_Attr_list->next;
            if (Traverse_Attr_list->index > LastAttrInd)
                    break;
            }
        }
    }
}