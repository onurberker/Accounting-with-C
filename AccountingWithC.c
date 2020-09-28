#include<stdio.h>
#include<unistd.h>
#include<string.h>
#include<ctype.h>
#include<conio.h>

typedef struct musteri{
	int hesapNo;
	char  isim[10];
	char  soyIsim[20];
	char telefonNo[10];
	double Bakiye;
}musteri;

void onYukleme(musteri mstr [])
{
	int i=1;
	while(i<=100)
	{
		mstr[i-1].hesapNo = -1;
		i++;
	}
}

dosyaYaz(musteri mstr[])
{
	FILE *fp;
	fp = fopen("musteriler.dat","wb");
	if(fp == NULL)
		exit(1);
	
	int i=1;
	while(i<=100)
	{
		fwrite(&mstr[i-1],sizeof(musteri),1,fp);
		i++;
	}	
	fclose(fp);
}


void okuDoldur(musteri mstr[])
{
	FILE *fp;
	fp = fopen("musteriler.dat","rb");
	if(fp == NULL)
		exit(1);
	
	int i=1;
	while(i<=100)
	{
		fread(&mstr[i-1], sizeof(musteri), 1, fp);
		i++;
	}
	fclose(fp);
}

void dosyaKontrolu(musteri mstr[])
{
	if( access( "musteriler.dat", F_OK ) != -1 )
	  okuDoldur(mstr);	
	else
	{
		onYukleme(mstr);
		dosyaYaz(mstr);
	}	
}

int gecerliHesapNo(char *str)
{
	int i;
	for( i = 1;  str[i] != '\0' ;i++)
	{
		if(!((int)str[i-1] >= 48 &&  (int)str[i-1] <=57))
			return -1;
	}	
	int hesappNo;
	sscanf(str, "%d", &hesappNo); 
	return (hesappNo>=1 && hesappNo <= 100) ? hesappNo : -1;			
}
int telefonNoKontrol(char * str)
{
	int i;
	if(str[0] == '0') return 0;
	
	for( i = 1;  str[i] != '\0' ;i++)
		if(!((int)str[i-1] >= 48 &&  (int)str[i-1] <=57))
			return 0;
			
	return 1;	
}

int gecerliMetinKontrolu(char *str)
{
	int i;
	for( i = 1;  str[i] != '\0' ;i++)
		if(!isalpha(str[i-1]) && str[i-1] != ' ')
			return 0;
	return 1;		
}


double gecerliBakiye(char * str)
{
	int i , baslangic = 1;
	if(str[0] == '-') 
		baslangic++;
	
	for( i = baslangic;  str[i] != '\0' ;i++)
		if(!isdigit(str[i-1]) && str[i-1] != '.' && str[i-1])
			return -1;
			
	double Bakiye;
	sscanf(str, "%lf", &Bakiye); 

	return (Bakiye>=-50000 && Bakiye <= 50000) ? Bakiye : -1;
}


int ismeGoreKarsilastir(const void* p, const void* q) 
{ 
     char * tempP =  ((struct musteri*)p)->isim;
     int i;
     for( i = 0; tempP[i]; i++)
	 {
  		tempP[i] = toupper(tempP[i]);
	 }
	 char * tempQ = ((struct musteri*)q)->isim;
	 for( i = 0; tempQ[i]; i++)
	 {
  		tempQ[i] = toupper(tempQ[i]);
	 }
	 
    return strcmp(tempP, tempQ); 
} 


int soyismeGoreKarsilastir(const void* p, const void* q) 
{ 
     char * tempP =  ((struct musteri*)p)->soyIsim;
     int i;
     for( i = 0; tempP[i]; i++)
	 {
  		tempP[i] = toupper(tempP[i]);
	 }
	 char * tempQ = ((struct musteri*)q)->soyIsim;
	 for( i = 0; tempQ[i]; i++)
	 {
  		tempQ[i] = toupper(tempQ[i]);
	 }
	 
    return strcmp(tempP, tempQ); 
} 

void bakiyeGuncelle(musteri mstr[])
{
	char  temp [50];
	int hesappNo;
	while(1)
	{
	printf("Hesap numaranizi giriniz. \n");
	gets(temp);
	hesappNo = gecerliHesapNo(temp);
	if(hesappNo != -1)
		break;
	else
	  	printf("Yanlis bir hesap numarasi girdiniz.\n");	
	}

	if(mstr[hesappNo-1].hesapNo == -1)  
	{
		printf("Hesap olusturulamadi.\n");
		return;
	}

	double Bakiye = mstr[hesappNo-1].Bakiye;	
	printf("Güncel bakiyeniz : %lf\n\n", Bakiye);
	printf("Para yükle ( 1 )\n");
	printf("Para çek   ( 2 )\n");
	
	int ch;
	scanf("%d",&ch);
	fflush(stdin);
	
    double miktar;
    double durum = 1;
	if(ch == 1)
		printf("Ne kadar para yatirmak istediginizi giriniz.\n");	
	else if( ch == 2)
	{
		printf("Ne kadar para çekmek istediginizi giriniz.\n");
		durum = -1;
	}
	else {
		printf("Geçersiz seçenek.\n\n");
		return;
	}
	scanf("%lf",&miktar);
	fflush(stdin);
	miktar *= durum;

	if(Bakiye + miktar >=-50000 && Bakiye + miktar <= 50000  && Bakiye + miktar != mstr[hesappNo-1].Bakiye )
	{
		mstr[hesappNo-1].Bakiye = Bakiye + miktar;
	  printf("Bakiyeniz güncellendi.\n");
	  printf("Yeni bakiyeniz : %lf\n\n",mstr[hesappNo-1].Bakiye);
	  dosyaYaz(mstr);	
	}
	else{
		printf("Bakiye güncellemeniz basarisiz oldu  (-50000,50000 arasi)\n\n");
	}
	
		
}

void yeniMusteriKaydi(musteri mstr[])
{
	char tempHesap[50];
	int hesappNo;
	while(1)
	{
		printf("Lütfen hesap numarasi seciniz.\n");
		gets(tempHesap);
		 hesappNo = gecerliHesapNo(tempHesap);
		if(hesappNo != -1)
			if(mstr[hesappNo-1].hesapNo == -1)
		    	break;
		    else 
				printf("Hesap numarasi baska biri tarafindan kullanilmaktadir.\n");	
		else
		printf("Yanlis bir hesap numarasi girdiniz.\n");
	}
	
	char  isim[50];
	while(1)
	{
		printf("Isim giriniz (en fazla 10 karakter)\n");
		gets(isim);
		if(strlen(isim) <=10 && strlen(isim) >= 1)
			if(!gecerliMetinKontrolu(isim))
				printf("Isminiz sadece karakter içermelidir\n\n");
			else
				break;
		else
			printf("Fazla karakter girdiniz.\n\n");
	}
	
	char soyIsim[50];
	while(1)
	{
		printf("Soyisim giriniz (en fazla 20 karakter)\n");
		gets(soyIsim);
		if(strlen(soyIsim) <=20 && strlen(soyIsim) >= 1)
			if(!gecerliMetinKontrolu(soyIsim))
				printf("Soyadiniz sadece karakter içermelidir.\n\n");
			else
				break;
		else
			printf("Fazla karakter girdiniz.\n\n");
	}
	char  tel [50];
	while(1)
	{
		printf("Telefon numarasi giriniz. (10 karakterden olusmalidir.)\n");
		gets(tel);
		if(strlen(tel) == 10 && telefonNoKontrol(tel))
			break;
		else
			printf("Yanlis telefon numarasi.\n\n");
	}
	
	char  tempBak [50];
	double Bakiye;
	while(1)
	{
		printf("Bakiye giriniz (-50000,50000 arasi)\n");
		gets(tempBak);
		Bakiye = gecerliBakiye(tempBak);
		if(Bakiye != -1.0)
			break;
		else
			printf("Geçersiz bakiye \n\n");
	}
	
	mstr[hesappNo-1].hesapNo = hesappNo;
	mstr[hesappNo-1].Bakiye = Bakiye;
	strcpy(mstr[hesappNo-1].isim , isim);
	strcpy(mstr[hesappNo-1].soyIsim , soyIsim);
	strcpy(mstr[hesappNo-1].telefonNo , tel);
	
	dosyaYaz(mstr);
	printf("Müsteri kaydi olusturulmustur.\n\n");
}

void kayitSil(musteri mstr[])
{
	char tempHesap[50];
	int hesappNo;
	while(1)
	{
		printf("Lütfen hesap numarasi seciniz.\n");
		gets(tempHesap);
		 hesappNo = gecerliHesapNo(tempHesap);
		if(hesappNo != -1)
			break;	
		else
		printf("Yanlis bir hesap numarasi girdiniz.\n");
	}
	
	if(mstr[hesappNo-1].hesapNo != -1)
	{
		printf("Müsteri kaydi silindi\n");
			mstr[hesappNo-1].hesapNo = -1;
			dosyaYaz(mstr);
	}
	else
		printf("Bu hesap numarasina kayitli bir kullanici yok\n");
		
	
	
}


void musteriGoster(musteri mstr[])
{
	char tempHesap[50];
	int hesappNo;
	while(1)
	{
		printf("Lütfen hesap numarasi seciniz.\n");
		gets(tempHesap);
		 hesappNo = gecerliHesapNo(tempHesap);
		if(hesappNo != -1)
			break;	
		else
		printf("Yanlis bir hesap numarasi girdiniz.\n");
	}
	
	if(mstr[hesappNo-1].hesapNo != -1)
	{
		printf("Hesap Numarasi:%d\n" ,mstr[hesappNo-1].hesapNo);
		printf("Isim:%s\n" ,mstr[hesappNo-1].isim);
		printf("Soyisim:%s\n" ,mstr[hesappNo-1].soyIsim);
		printf("Telefon:%s\n" ,mstr[hesappNo-1].telefonNo);
		printf("Bakiye:%lf\n\n" ,mstr[hesappNo-1].Bakiye);	
	}
	else
		printf("Bu hesap numarasina kayitli bir musteri yok\n\n");
}

void hesapNoyaGoreSirala(musteri mstr[])
{
	FILE *fp ; 
	
	fp = fopen("hesap_no_sirali.txt","w");
	if(fp ==NULL)
	exit(1);
	int i=1;
	while(i<=100)
	{
		if(mstr[i-1].hesapNo != -1)
		{
		fprintf(fp,"Hesap Numarasi:%d\n" ,mstr[i-1].hesapNo);
		fprintf(fp,"Isim:%s\n" ,mstr[i-1].isim);
		fprintf(fp,"Soyisim:%s\n" ,mstr[i-1].soyIsim);
		fprintf(fp,"Telefon Numarasi:%s\n" ,mstr[i-1].telefonNo);
		fprintf(fp,"Bakiye:%lf\n\n" ,mstr[i-1].Bakiye);	
		}
		i++;
	}
	fclose(fp);
	printf("Basariyla olusturuldu.\n");
		
}

void ismeGoreSirala(musteri mstr[])
{
	int sayac=0,j,k;
	int i=1;
	while(i<=100)
	{
		if(mstr[i-1].hesapNo != -1)
			sayac ++;
	
	i++;
	}
	musteri sirali[sayac];
	for(i = 1,k=1; i<=100;i++)
		if(mstr[i-1].hesapNo != -1)
			{
				sirali[k-1].hesapNo = mstr[i-1].hesapNo;
				sirali[k-1].Bakiye = mstr[i-1].Bakiye;
				strcpy(sirali[k-1].isim , mstr[i-1].isim);
				strcpy(sirali[k-1].soyIsim , mstr[i-1].soyIsim);
				strcpy(sirali[k-1].telefonNo , mstr[i-1].telefonNo);
				k++;
			}
	

	qsort(sirali, sayac, sizeof(struct musteri), ismeGoreKarsilastir); 
            
	FILE *fp ; 
	
	fp = fopen("isme_gore_sirali.txt","w");
	if(fp ==NULL)
	exit(1);
	i=1;
	while(i<=sayac)
	{
		fprintf(fp,"Hesap Numarasi:%d\n" ,sirali[i-1].hesapNo);
		fprintf(fp,"Isim:%s\n" ,sirali[i-1].isim);
		fprintf(fp,"Soyisim:%s\n" ,sirali[i-1].soyIsim);
		fprintf(fp,"Telefon Numarasi:%s\n" ,sirali[i-1].telefonNo);
		fprintf(fp,"Bakiye:%lf\n\n" ,sirali[i-1].Bakiye);
		i++;
	}
	fclose(fp);
	
	printf("Basariyla olusturuldu.\n");
	
}

void soyismeGoreSirala(musteri mstr[])
{
	int sayac=0,j,k;
	int i=1;
	while(i<=100)
	{
		if(mstr[i-1].hesapNo != -1)
			sayac ++;

	i++;
	}
		musteri sirali[sayac];
	for(i = 1,k=1; i<=100;i++)
		if(mstr[i-1].hesapNo != -1)
			{
				sirali[k-1].hesapNo = mstr[i-1].hesapNo;
				sirali[k-1].Bakiye = mstr[i-1].Bakiye;
				strcpy(sirali[k-1].isim , mstr[i-1].isim);
				strcpy(sirali[k-1].soyIsim , mstr[i-1].soyIsim);
				strcpy(sirali[k-1].telefonNo , mstr[i-1].telefonNo);
				k++;
			}

	qsort(sirali, sayac, sizeof(struct musteri), soyismeGoreKarsilastir); 
            
	FILE *fp ; 
	
	fp = fopen("sirali_by_soyIsim.txt","w");
	if(fp ==NULL)
	exit(1);
	i=1;
	while(i<=sayac)
	{
		fprintf(fp,"Hesap Numarasi:%d\n" ,sirali[i-1].hesapNo);
		fprintf(fp,"Isim:%s\n" ,sirali[i-1].isim);
		fprintf(fp,"Soyisim:%s\n" ,sirali[i-1].soyIsim);
		fprintf(fp,"Telefon Numarasi:%s\n" ,sirali[i-1].telefonNo);
		fprintf(fp,"Bakiye:%lf\n\n" ,sirali[i-1].Bakiye);
		i++;
	}		
	fclose(fp);
	printf("Basariyla olusturuldu.\n");
}

void siralamaSec(musteri  mstr [])
{
	
	printf("Hesap numarasina göre sirala ( 1 )\n");
	printf("Isme göre sirala( 2 )\n");
	printf("Soyisme göre sirala ( 3 )\n");
	int ch;
	scanf("%d",&ch);
	fflush(stdin);
	if(ch == 1)
	hesapNoyaGoreSirala(mstr);
	else if(ch == 2)
	ismeGoreSirala(mstr);
	else if(ch == 3)
	soyismeGoreSirala(mstr);
	else printf("Yanlis Secenek\n");
	
	
}
void borcluFiltre(musteri mstr[])
{
	int sayac=0,j,k;
	int i=1;
	while(i<=100)
	{
		if(mstr[i-1].hesapNo != -1 && mstr[i-1].Bakiye < 0)
			sayac ++;
	
	i++;
	}
	musteri filtre[sayac];
	for(i = 1,k=1; i<=100;i++)
		if(mstr[i-1].hesapNo != -1 &&  mstr[i-1].Bakiye < 0 )
			{
				filtre[k-1].hesapNo = mstr[i-1].hesapNo;
				filtre[k-1].Bakiye = mstr[i-1].Bakiye;
				strcpy(filtre[k-1].isim , mstr[i-1].isim);
				strcpy(filtre[k-1].soyIsim , mstr[i-1].soyIsim);
				strcpy(filtre[k-1].telefonNo , mstr[i-1].telefonNo);
				k++;
			}
	FILE *fp ; 
	
	fp = fopen("borclular_filtre.txt","w");
	if(fp ==NULL)
	exit(1);
	i=1;
	while(i<=sayac)
	{
		fprintf(fp,"Hesap Numarasi:%d\n" ,filtre[i-1].hesapNo);
		fprintf(fp,"Isim:%s\n" ,filtre[i-1].isim);
		fprintf(fp,"Soyisim:%s\n" ,filtre[i-1].soyIsim);
		fprintf(fp,"Telefon Numarasi:%s\n" ,filtre[i-1].telefonNo);
		fprintf(fp,"Bakiye:%lf\n\n" ,filtre[i-1].Bakiye);
		i++;
	}
	fclose(fp);
	printf("Basariyla olusturuldu\n");
}


void borcluOlmayanFiltre(musteri mstr[])
{
	int sayac=0,j,k;
	int i=1;
	while(i<=100)
	{
		if(mstr[i-1].hesapNo != -1 && mstr[i-1].Bakiye >= 0)
			sayac ++;
	i++;
	}
	musteri filtre[sayac];
	for(i = 1,k=1; i<=100;i++)
		if(mstr[i-1].hesapNo != -1 &&  mstr[i-1].Bakiye >= 0 )
			{
				filtre[k-1].hesapNo = mstr[i-1].hesapNo;
				filtre[k-1].Bakiye = mstr[i-1].Bakiye;
				strcpy(filtre[k-1].isim , mstr[i-1].isim);
				strcpy(filtre[k-1].soyIsim , mstr[i-1].soyIsim);
				strcpy(filtre[k-1].telefonNo , mstr[i-1].telefonNo);
				k++;
			}
			
	FILE *fp ; 
	fp = fopen("borclu_olmayan_filtrelenen.txt","w");
	if(fp ==NULL)
	exit(1);
	i=1;
	while(i<=sayac)
	{
		fprintf(fp,"Hesap Numarasi:%d\n" ,filtre[i-1].hesapNo);
		fprintf(fp,"Isim:%s\n" ,filtre[i-1].isim);
		fprintf(fp,"Soyisim:%s\n" ,filtre[i-1].soyIsim);
		fprintf(fp,"Telefon Numarasi:%s\n" ,filtre[i-1].telefonNo);
		fprintf(fp,"Bakiye:%lf\n\n" ,filtre[i-1].Bakiye);
		i++;
	}
	fclose(fp);
	printf("Basariyla olusturuldu.\n");		
}
void tumunuFiltrele(musteri mstr[])
{
	FILE *fp;
	fp = fopen("filtrelenen_tumu.txt","w");
	if(fp ==NULL)
	exit(1);

 	int i=1;
 	while(i<=100)
 	{
 			if(mstr[i-1].hesapNo != -1)
		{
		fprintf(fp,"Hesap Numarasi:%d\n" ,mstr[i-1].hesapNo);
		fprintf(fp,"Isim:%s\n" ,mstr[i-1].isim);
		fprintf(fp,"Soyisim:%s\n" ,mstr[i-1].soyIsim);
		fprintf(fp,"Telefon Numarasi:%s\n" ,mstr[i-1].telefonNo);
		fprintf(fp,"Bakiye:%lf\n\n" ,mstr[i-1].Bakiye);		
		}
		i++;
	}
	fclose(fp);
	printf("Basariyla olusturuldu.\n");
}

void filtreleme(musteri mstr[])
{
    printf("Borclulari Filtrele ( 1 )\n");
    printf("Borclu olmayanlari Filtrele ( 2 )\n");
    printf("Borclu olmayanlari ve borclulari Fitrele(Enter 3)\n");
    int ch ;
	scanf("%d",&ch);
	fflush(stdin);
	if(ch == 1)
	borcluFiltre(mstr);
	else if(ch == 2)
	borcluOlmayanFiltre(mstr);
	else if(ch == 3)
	tumunuFiltrele(mstr);
	else printf("Yanlis Secenek.\n");
    
}

void menu (musteri mstr[])
{

	while(1)
	{		
    printf("\n---------------------------------\n");
	printf("-Bakiye Güncelle-( 1 )\n");
	printf("-Yeni Kayit- ( 2 )\n");
	printf("-Kayit Sil- ( 3 )\n");
	printf("-Müsteri Görüntüle- ( 4 )\n");
	printf("-Siralama Menüsü-  ( 5 )\n");
	printf("-Filtreleme Menüsü- ( 6 )\n");
	printf("-Cikis- ( 0 )\n");
	printf("\n----------------------------------\n");
	int ch;
	scanf("%d",&ch);
	fflush(stdin);	
	switch(ch)
	{
		case 1 :{
			bakiyeGuncelle(mstr);
			break;
		}
		case 2 :{
			yeniMusteriKaydi(mstr);
			break;
		}
		case 3 :{
			kayitSil(mstr);
			break;
		}
		case 4 :{
			musteriGoster(mstr);
			break;
		}
		case 5 :{
			siralamaSec(mstr);
			break;
		}
		case 6 :{
			filtreleme(mstr);
			break;
		}
		case 0 :{
			return;
		}
		default:{
			printf("Yanlis Secenek.\n");
			break;
		}	
	}
}
	
}
main()
{
	musteri mstr[100];
	dosyaKontrolu(mstr);
	menu(mstr);
}
