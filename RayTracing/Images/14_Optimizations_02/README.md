***CPU*** - *AMD Ryzen 7 5800H*

| ***Scene*** | ***Render Mode*** | ***Optimizations*** | ***Time*** |
| --- | --- | --- | --- |
| Eight Triangles | `Buckets` | No | 269 ms |
| Eight Triangles | `Buckets` | *KDTree* | 240 ms |
| Dragon | `Buckets` | No | 67753 ms |
| Dragon | `Buckets` | *KDTree* | 1079 ms |
| WALL-E | `Buckets` | *KDTree* | 8070 ms |
| Refractive WALL-E | `Buckets` | *KDTree* | 69628 ms |

> [!NOTE]
> Dragon scene has 4014 triangles, while WALL-E has above 64000 triangles
