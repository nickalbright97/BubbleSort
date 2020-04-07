int[][][] find_dist(int *g, int w, int h, int d)
{
    int dist[w][h][d];
    struct queue *worker_queue;
    int width;

    worker_queue = (struct queue *)malloc(sizeof(struct queue));
    initqueue(worker_queue);
    width = w * h * d;

    for (int x = 0; x < w; x++)
    {
        for (int y = 0; y < y; y++)
        {
            for (int z = 0; z < d; z++)
            {
                if (indexArray(g, width, x, y, z) != 0)
                {
                    dist[x][y][z] = -1;
                }
                else
                {
                    struct cell *c = create_element(x, y, z, 0);
                    enqueue(worker_queue, c);
                }
            }
        }
    }

    for (int i = 1; isempty(worker_queue) != 1; i++)
    {
     	int n = worker_queue->size;
        for (int j = 0; j < n; j++)
        {
            struct cell *c;
            c = dequeue(worker_queue);

            if (c->x-1 > -1 && dist[c->x-1][c->y][c->z] == -1)
            {
             	dist[c->x-1][c->y][c->z] = i;
                enqueue(worker_queue, create_element(c->x-1, c->y, c->z, dist[c->x-1][c->y][c->z]));
            }
            if (c->x+1 < w && dist[c->x+1][c->y][c->z] == -1)
            {
             	dist[c->x+1][c->y][c->z] = i;
                enqueue(worker_queue, create_element(c->x+1, c->y, c->z, dist[c->x+1][c->y][c->z]));
            }
            if (c->y-1 > -1 && dist[c->x][c->y-1][c->z] == -1)
            {
             	dist[c->x][c->y-1][c->z] = i;
                enqueue(worker_queue, create_element(c->x, c->y-1, c->z, dist[c->x][c->y-1][c->z]));
            }
            if (c->y+1 < h && dist[c->x][c->y+1][c->z] == -1)
            {
             	dist[c->x][c->y+1][c->z] = i;
                enqueue(worker_queue, create_element(c->x, c->y+1, c->z, dist[c->x][c->y+1][c->z]));
            }
            if (c->z-1 > -1 && dist[c->x][c->y][c->z-1] == -1)
            {
             	dist[c->x][c->y][c->z-1] = i;
                enqueue(worker_queue, create_element(c->x, c->y, c->z-1, dist[c->x][c->y][c->z-1]));
            }
            if (c->z+1 < d && dist[c->x][c->y][c->z+1] == -1)
            {
             	dist[c->x][c->y][c->z+1] = i;
                enqueue(worker_queue, create_element(c->x, c->y, c->z+1, dist[c->x][c->y][c->z+1]));
            }

            free(c);
        }
    }

    free(worker_queue);

    return dist;
}
