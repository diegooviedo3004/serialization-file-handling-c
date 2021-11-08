#include <stdio.h>
#include <string.h>
#include <conio.h>
#include <unistd.h>
#include <Windows.h>

/*DESARROLLADO POR DIEGO ENRIQUE OVIEDO LLANES github: diegooviedo3004*/
typedef struct producto
{
    char codigo_producto[20];
    char nombre_producto[20];
    int precio_unidad;
    int total_unidades;

} producto;

typedef struct trabajador
{
    char codigo_trabajador[20];
    char nombre_trabajador[20];

} trabajador;

typedef struct venta
{
    char cod_vendedor_v[20];
    char cod_producto_v[20];
    int cantidad_vendida;
    char hora_venta[20];
} venta;

int get_product_code(char op);
void menu();
void almacen();
void ingresar_producto();
void format_codigo(char op, char *str);
void lista_productos();
void trabajadores();
void ingresar_trabajador();
void aumentar_codigo_t();
void format_codigo_t(char *str);
void lista_trabajadores();
void ventas();
int validar_nombre(char *str, char *str2);
int validar_producto(char *str, char *str2);
int validar_cantidad();
void cambiar_almacen(int cantidad, char *nombre_producto);
void ingresar_venta();
void ver_lista_ventas();
void reporte();
int get_nombre_vendedor(char *str,char *str2);
int get_nombre_producto(char *str,char *str2,int* cost_unidad);
void subreporte();
int checkear_nombre(char *str);
void actualizar_total_unidades(int cantidad_nueva, char *str);
int checkear_nombre_trabajador(char *str);
void total_vendedor();
void total_producto();
void total_area();

int main()
{
    menu();
    return 0;
}

void menu()
{
    char op;
    fflush(stdin);
    system("cls");
    printf("----MENU----\n1.Almacen\n2.Trabajador\n3.Ventas\n4.Reporte\n5.Subreporte\n6.Salir\nOpcion: ");
    scanf("%c", &op);
    switch (op)
    {
    case '1':
        almacen();
        break;

    case '2':
        trabajadores();
        break;

    case '3':
        ventas();
        break;

    case '4':
        reporte();
        break;

    case '5':
        subreporte();
        break;

    case '6':
        exit(1);
        break;
    default:
        system("cls");
        printf("Ingrese una opcion valida");
        sleep(1);
        menu();
    }
}

void almacen()
{
    char op;
    system("cls");
    printf("----ALMACEN----\n1.Ingresar producto\n2.Ver lista de productos\n3.Volver al menu\nOpcion: ");
    fflush(stdin);
    scanf("%c", &op);
    switch (op)
    {
    case '1':
        ingresar_producto();
        break;
    case '2':
        lista_productos();
        break;
    case '3':
        menu();
        break;
    default:
        system("cls");
        printf("Ingrese una opcion valida");
        sleep(1);
        almacen();
    }
}
int get_product_code(char op)
{
    int codigos[3];
    FILE *file = fopen("cc.txt", "r");
    if (file == NULL)
    {
        return 1;
    }
    fscanf(file, "%d\n%d\n%d", &codigos[0], &codigos[1], &codigos[2]);
    if (op == '1')
    {
        return codigos[0];
    }
    else if (op == '2')
    {
        return codigos[1];
    }
    else
    {
        return codigos[2];
    }
    fclose(file);
}

void ingresar_producto()
{
    char op;
    char str[20];
    FILE *file = fopen("almacen.txt", "a");
    producto *p;
    int n, i;
    system("cls");
    printf("----TIPO DE PRODUCTO----\n1.Electrico\n2.Calzado\n3.Vestuario\n4.Volver al menu\nOpcion: ");
    fflush(stdin);
    scanf("%c", &op);
    printf("\nIngrese cuantos productos desea ingresar: ");
    fflush(stdin);
    scanf("%d", &n);

    p = (producto *)calloc(n, sizeof(producto));
    for (i = 0; i < n; i++)
    {
        format_codigo(op, str);
        strcpy(p[i].codigo_producto, str);
        fflush(stdin);
        printf("\nIngrese nombre del producto: ");
        scanf("%[^\n]s", p[i].nombre_producto);
        fflush(stdin);
        //Nombre no exista
        if(checkear_nombre(p[i].nombre_producto)){
            printf("\nEl producto se ha detectado en la base de datos\n");
            printf("\nCantidad de nuevas unidades a ingresar : ");
            scanf("%d", &p[i].total_unidades);
            actualizar_total_unidades(p[i].total_unidades,p[i].nombre_producto);            
        }else{
            printf("\nIngrese precio por unidad: ");
            scanf("%d", &p[i].precio_unidad);
            fflush(stdin);
            printf("\nCantidad de unidades a ingresar : ");
            scanf("%d", &p[i].total_unidades);
            fflush(stdin);
            fwrite(&p[i], sizeof(producto), 1, file);
        }
        
    }
    fclose(file);
    system("cls");
    printf("Productos agregados\nVolviendo al menu");
    sleep(1);
    menu();
}

void actualizar_total_unidades(int cantidad_nueva, char *str){
    producto p1;
    FILE *file = fopen("almacen.txt", "r");
    FILE *file2 = fopen("almacen-tmp.txt", "w");
    while (fread(&p1, sizeof(producto), 1, file))
    {
        if (strcmp(p1.nombre_producto, str) == 0)
        {
            p1.total_unidades += cantidad_nueva;
            fwrite(&p1, sizeof(producto), 1, file2);
        }else{
            fwrite(&p1, sizeof(producto), 1, file2);
        }
    }
    fclose(file);
    fclose(file2);

    file = fopen("almacen.txt","w");
    file2 = fopen("almacen-tmp.txt","r");
    while (fread(&p1, sizeof(producto), 1, file2))
    {
        fwrite(&p1,sizeof(producto),1,file);
    }

    fclose(file);
    fclose(file2);
}

//RETORNA TRUE SI EL NOMBRE EXISTE, FALSE SI NO
int checkear_nombre(char *str){
    producto p1;
    FILE *file = fopen("almacen.txt", "r");
    while (fread(&p1, sizeof(producto), 1, file))
    {
        if(strcmp(p1.nombre_producto,str) == 0){
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void format_codigo(char op, char *str)
{
    if (op == '1')
    {
        sprintf(str, "ELEC-%.5d", get_product_code(op));
    }
    else if (op == '2')
    {
        sprintf(str, "CAL-%.5d", get_product_code(op));
    }
    else
    {
        sprintf(str, "VES-%.5d", get_product_code(op));
    }
}

void lista_productos()
{
    producto p1;
    FILE *file = fopen("almacen.txt", "r");
    system("cls");
    printf("NOMBRE     CANTIDAD\tPRECIO_UNIDAD");
    while (fread(&p1, sizeof(producto), 1, file))
    {
        if(p1.total_unidades > 0){
           printf("\n%s\t%d\t%d", p1.nombre_producto, p1.total_unidades, p1.precio_unidad);
        }
    }
    fclose(file);
    printf("\n\nPresione cualquier tecla para volver al menu");
    getch();
    menu();
}

void trabajadores()
{
    char op;
    system("cls");
    printf("----TRABAJADORES----\n1.Ingresar trabajador\n2.Ver lista de trabajadores\n3.Volver al menu\nOpcion: ");
    fflush(stdin);
    scanf("%c", &op);
    switch (op)
    {
    case '1':
        ingresar_trabajador();
        break;
    case '2':
        lista_trabajadores();
        break;
    case '3':
        menu();
        break;
    default:
        system("cls");
        printf("Ingrese una opcion valida");
        sleep(1);
        trabajadores();
    }
}

void ingresar_trabajador()
{
    char str[20];
    FILE *file = fopen("trabajadores.txt", "a");
    trabajador *t;
    int n, i;
    system("cls");
    fflush(stdin);
    printf("\nIngrese cuantos trabajadores desea ingresar: ");
    fflush(stdin);
    scanf("%d", &n);

    t = (trabajador *)calloc(n, sizeof(trabajador));

    for (i = 0; i < n; i++)
    {
        format_codigo_t(str);
        strcpy(t[i].codigo_trabajador, str);
        printf("\nIngrese nombre del trabajador: ");
        fflush(stdin);
        scanf("%[^\n]s", t[i].nombre_trabajador);
        if(checkear_nombre_trabajador(t[i].nombre_trabajador)){
            printf("\nEl trabajador ya existe\n");
            printf("\nIngrese otro nombre");
            sleep(1);
            trabajadores();
        }else{
            fflush(stdin);
            fwrite(&t[i], sizeof(trabajador), 1, file);
            aumentar_codigo_t();
        }
    }
    fclose(file);
    system("cls");
    printf("Trabajador/es agregados\nVolviendo al menu");
    sleep(1);
    menu();
}

int checkear_nombre_trabajador(char *str){
    trabajador t1;
    FILE *file = fopen("trabajadores.txt", "r");
    while (fread(&t1, sizeof(trabajador), 1, file))
    {
        if(strcmp(str,t1.nombre_trabajador) == 0){
            return 1;
        }
    }
    fclose(file);
    return 0;
}
void format_codigo_t(char *str)
{
    int actual;
    FILE *file = fopen("cct.txt", "r");
    fscanf(file, "%d", &actual);
    fclose(file);

    sprintf(str, "V-%.2d", actual);
}

void aumentar_codigo_t()
{
    char num[10];
    int actual;
    FILE *file = fopen("cct.txt", "r");
    fscanf(file, "%d", &actual);
    fclose(file);
    actual = actual + 1;
    sprintf(num, "%d", actual);
    file = fopen("cct.txt", "w");
    fputs(num, file);
    fclose(file);
}

void lista_trabajadores()
{
    trabajador t1;
    int codigo;
    char str[20];
    FILE *file = fopen("trabajadores.txt", "r");
    system("cls");
    printf("NUMERO    NOMBRE");
    while (fread(&t1, sizeof(trabajador), 1, file))
    {
        strcpy(str, t1.codigo_trabajador);
        sscanf(str, "V-%d", &codigo);
        printf("\n%d\t%s", codigo, t1.nombre_trabajador);
    }
    fclose(file);
    printf("\n\nPresione cualquier tecla para volver al menu");
    getch();
    menu();
}

void ventas()
{
    char op;
    system("cls");
    printf("----VENTAS----\n1.Ingresar venta\n2.Volver al menu\nOpcion: ");
    fflush(stdin);
    scanf("%c", &op);
    switch (op)
    {
    case '1':
        ingresar_venta();
        getch();
        break;

    case '2':
        menu();
        break;
    default:
        system("cls");
        printf("Ingrese una opcion valida");
        sleep(1);
        ventas();
    }
}

int validar_nombre(char *str, char *str2)
{
    trabajador t1;
    FILE *file = fopen("trabajadores.txt", "r");
    while (fread(&t1, sizeof(trabajador), 1, file))
    {
        if (strcmp(str, t1.nombre_trabajador) == 0)
        {
            strcpy(str2, t1.codigo_trabajador);
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int validar_producto(char *str, char *str2)
{
    producto p1;
    FILE *file = fopen("almacen.txt", "r");
    while (fread(&p1, sizeof(producto), 1, file))
    {
        if (strcmp(p1.nombre_producto, str) == 0 && p1.total_unidades > 0)
        {
            fclose(file);
            strcpy(str2, p1.codigo_producto);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

int validar_cantidad(char *str, int cantidad)
{
    producto p1;
    FILE *file = fopen("almacen.txt", "r");
    while (fread(&p1, sizeof(producto), 1, file))
    {
        if (strcmp(p1.nombre_producto, str) == 0 && p1.total_unidades >= cantidad)
        {
            fclose(file);
            return 1;
        }
    }
    fclose(file);
    return 0;
}

void cambiar_almacen(int cantidad, char *nombre_producto)
{
    producto p1;
    int result;
    char str[20];
    int cambio = 0;
    FILE *file = fopen("almacen.txt", "r");
    FILE *file2 = fopen("almacen-tmp.txt", "w");
    while (fread(&p1, sizeof(producto), 1, file))
    {
        if (strcmp(p1.nombre_producto, nombre_producto) == 0)
        {
            cambio = p1.total_unidades;
            cambio = cambio - cantidad;
            p1.total_unidades = cambio;
            fwrite(&p1, sizeof(producto), 1, file2);
        }else{
            fwrite(&p1, sizeof(producto), 1, file2);
        }
    }
    fclose(file);
    fclose(file2);
   
    file = fopen("almacen.txt","w");
    file2 = fopen("almacen-tmp.txt","r");
    while (fread(&p1, sizeof(producto), 1, file2))
    {
        fwrite(&p1,sizeof(producto),1,file);
    }

    fclose(file);
    fclose(file2);
}

void ingresar_venta()
{
    char nombre[20], nombre_producto[20];
    int cantidad;
    FILE *file = fopen("ventas.txt", "a");
    venta *v;
    int n, i;
    system("cls");
    fflush(stdin);
    printf("\nIngrese cuantos ventas desea ingresar: ");
    fflush(stdin);
    scanf("%d", &n);

    v = (venta *)calloc(n, sizeof(venta));

    for (i = 0; i < n; i++)
    {
        fflush(stdin);
        printf("\nIngrese el nombre del vendedor: ");
        scanf("%[^\n]s", nombre);
        if (validar_nombre(nombre, v[i].cod_vendedor_v))
        {
            printf("Ingrese nombre del producto: ");
            fflush(stdin);
            scanf("%[^\n]s", nombre_producto);
            if (validar_producto(nombre_producto, v[i].cod_producto_v))
            {
                printf("Ingrese cantidad vendida: ");
                fflush(stdin);
                scanf("%d", &cantidad);
                fflush(stdin);
                if (validar_cantidad(nombre_producto, cantidad))
                {
                    v[i].cantidad_vendida = cantidad;
                    printf("Ingrese fecha: ");
                    fflush(stdin);
                    scanf("%s", v[i].hora_venta);
                    fflush(stdin);
                    fwrite(&v[i], sizeof(venta), 1, file);
                    cambiar_almacen(cantidad, nombre_producto);
                    break;
                }
                else
                {
                    system("cls");
                    printf("No hay suficientes existencias de este producto");
                    sleep(1);
                    ventas();
                }
            }
            else
            {
                system("cls");
                printf("El producto no existe, o ya no hay existencias");
                sleep(1);
                ventas();
            }
        }
        else
        {
            system("cls");
            printf("Ingrese el nombre de un vendedor valido");
            sleep(1);
            ventas();
        }
    }
    fclose(file);
    system("cls");
    printf("Venta/s agregadas\nVolviendo al menu");
    sleep(1);
    menu();
}

void reporte(){
    int total = 0,cost_unidad,venta_;
    char nombre[20],nombre_producto[20];
    venta v1;
    system("cls");
    printf("VENTAS DEL DIA 'GALLO MAS GALLO'\n");
    printf("VENDEDOR PRODUCTO CANTIDAD VENDIDA COST.UNIDAD VENTA\n");
    FILE *file = fopen("ventas.txt", "r");
    while (fread(&v1, sizeof(venta), 1, file))
    {
       get_nombre_vendedor(v1.cod_vendedor_v,nombre);
       get_nombre_producto(v1.cod_producto_v,nombre_producto,&cost_unidad);
       venta_ = cost_unidad * v1.cantidad_vendida;
       total += venta_;
       printf("%s %s %d %d %d\n",nombre,nombre_producto,v1.cantidad_vendida,cost_unidad,venta_);
    }
    fclose(file);
    printf("\n\nTOTAL VENTAS DEL DIA----------------------------%d",total);
    printf("\n\nPresione cualquier tecla para volver al menu");
    getch();
    menu();
}

int get_nombre_vendedor(char *str,char *str2){
    trabajador t1;
    FILE *file = fopen("trabajadores.txt", "r");
    while (fread(&t1, sizeof(trabajador), 1, file))
    {
        if(strcmp(str,t1.codigo_trabajador) == 0){
            strcpy(str2,t1.nombre_trabajador);
            return 1;
        }
    }
    fclose(file);
    return 1;
}

int get_nombre_producto(char *str,char *str2,int* cost_unidad){
    producto p1;
    FILE *file = fopen("almacen.txt", "r");
    while (fread(&p1, sizeof(producto), 1, file))
    {
        if(strcmp(p1.codigo_producto,str) == 0){
            strcpy(str2,p1.nombre_producto);
            *cost_unidad = p1.precio_unidad;
            return 1;
        }
    }

    fclose(file);
    return 0;
}

void subreporte(){
    char op;
    system("cls");
    printf("----SUBREPORTE----\n1.Total Vendedor\n2.Total Producto\n3.Total Area\nOpcion: ");
    fflush(stdin);
    scanf("%c", &op);
    switch (op)
    {
    case '1':
        total_vendedor();
        break;

    case '2':
        total_producto();
        break;
    
    case '3':
        total_area();
        break;

    default:
        system("cls");
        printf("Ingrese una opcion valida");
        sleep(1);
        subreporte();
    }
}

void total_vendedor(){
    char nombre[20];
    int contador = 0;
    venta v1;
    trabajador t1;
    system("cls");
    FILE *file = fopen("trabajadores.txt", "r");
    FILE *file2;
    printf("Trabajador------VENTAS\n\n");
    while (fread(&t1, sizeof(trabajador), 1, file))
    {
        file2 = fopen("ventas.txt", "r");
        contador = 0;
        while (fread(&v1, sizeof(venta), 1, file2))
        {
            get_nombre_vendedor(v1.cod_vendedor_v,nombre);
            if(strcmp(t1.nombre_trabajador,nombre) == 0){
                contador = contador + v1.cantidad_vendida;
            }
        }
        printf("%s : %d\n",t1.nombre_trabajador,contador);
        fclose(file2);
    }
    fclose(file);
    printf("\n\nPresione cualquier tecla para volver al menu");
    getch();
    menu();
}

void total_producto(){
    producto p1;
    int actual,contador;
    char nombre[20];
    venta v1;
    FILE *file = fopen("almacen.txt", "r");
    FILE *file2;
    system("cls");
    printf("PRODUCTO----VENTAS\n");
    while (fread(&p1, sizeof(producto), 1, file))
    {
        file2 = fopen("ventas.txt","r");
        contador = 0;
        while (fread(&v1, sizeof(venta), 1, file2))
        {
            get_nombre_producto(v1.cod_producto_v,nombre,&actual);
            if(strcmp(nombre,p1.nombre_producto) == 0){
                contador += v1.cantidad_vendida;
            }
            
        }
        printf("%s : %d\n",p1.nombre_producto,contador);
        fclose(file2);
    }
    fclose(file);
    printf("\n\nPresione cualquier tecla para volver al menu");
    getch();
    menu();
}

void total_area(){
    venta v1;
    int cont1 = 0,cont2 = 0,cont3 = 0;
    FILE *file2;
    file2 = fopen("ventas.txt","r");
    system("cls");
    printf("AREA VENTAS\n");
    while (fread(&v1, sizeof(venta), 1, file2))
    {
        if(v1.cod_producto_v[0] == 'E'){
            cont1++;
        }else if(v1.cod_producto_v[0] == 'C'){
            cont2++;
        }else{
            cont3++;
        }
    }
    printf("ELECTRONICOS : %d\nCALZADO : %d\nVESTUARIO : %d\n",cont1,cont2,cont3);
    fclose(file2);
    printf("\n\nPresione cualquier tecla para volver al menu");
    getch();
    menu();
}

