 
#include <dirent.h>
#include <stdio.h>
#include <string.h>
#include "jrb.h"
#include <sys/types.h>
#include "fields.h"
#include <unistd.h>
#include <stdlib.h>


#include "pro.h"

JRB siparisler, tmp;

//PROMPT makro olarak h dosyasına yazıldı.
void Prompt()
{
  printf(PROMPT);
}


typedef struct {
  int sipKey;
  char *sipAd;
  char *sipMalzeme;
  char *sipRenk;
} Siparis;

int Add(char **komutlar)
{
	int komut;
	//Argüman sayısı kontrolü
	for(komut = 0; komut <= 4; komut++)
	{
		if(komutlar[komut] == NULL)
		{
			printf(ERROR "Bicim uygun degil\n" );
			return 0;
		}
	}
	//Key'in sayi olma durumu kontrolü
	if(!IsCharIsNumber(komutlar[1]))
	{
		printf(ERROR "Bicim uygun degil\n" );
		return 0;
	}

	int nsize, i;
	//Her sipariş propertysi için bellek atamaları yapılıyor ve yapıya ekleniyor.
	Siparis *siparis;
	siparis = malloc(sizeof(Siparis));
	siparis->sipKey = atoi(komutlar[1]);

	nsize = strlen(komutlar[2]);
	siparis->sipAd = (char *) malloc(sizeof(char)*(nsize+1));
	strcpy(siparis->sipAd, komutlar[2]);

	nsize = strlen(komutlar[3]);
	siparis->sipMalzeme = (char *) malloc(sizeof(char)*(nsize+1));
	strcpy(siparis->sipMalzeme, komutlar[3]);

	nsize = strlen(komutlar[4]);
	siparis->sipRenk = (char *) malloc(sizeof(char)*(nsize+1));
	strcpy(siparis->sipRenk, komutlar[4]);

	//Key'in daha önce mevcut olup olmadığının kontrolü yapılıyor.
	tmp = jrb_find_int(siparisler, siparis->sipKey);
	if (tmp == NULL) 
	{
		jrb_insert_int(siparisler, siparis->sipKey, new_jval_v((void *) siparis));
		return 1;
	} 
	printf(ERROR "key:%d zaten mevcut\n", siparis->sipKey);
	return 0;

}

int Search(char **komutlar)
{
	Siparis *siparis;
	//Verilen key'in sayi olma durumu kontrolü yapılıyor.
	if(!IsCharIsNumber(komutlar[1]))
	{
		printf(ERROR "Key bir sayi olmali.\n");
		return 0;
	}
	int siparisKey = atoi(komutlar[1]);

	//Ağaçta key aranıyor.
	tmp = jrb_find_int(siparisler, siparisKey);
	if (tmp == NULL) 
	{
		printf(ERROR "Boyle bir siparis yok\n");
		return 0;
	}

	siparis = (Siparis *) tmp->val.v;
	printf("%d, %s, %s, %s\n",siparis->sipKey, siparis->sipAd, siparis->sipMalzeme, siparis->sipRenk);
	return 1;
}

int Write(char **komutlar)
{

	DIR *d;
	d = opendir(".");
	FILE *fp;
	//output verilen dosyaya yönlendiriliyor.
	fp = fopen(komutlar[1], "w");
	//argüman yoksa NULL döner.
	if (fp == NULL){
		printf(ERROR "Dosya belirtin\n");
		return 0;
	}
	jrb_traverse(tmp, siparisler) {
		Siparis *siparis = (Siparis *) tmp->val.v;

		fprintf(fp, "%d, %s, %s, %s\n",siparis->sipKey, siparis->sipAd, siparis->sipMalzeme, siparis->sipRenk);
  }
	fclose(fp);
  	closedir(d);
	return 1;
}

int Print()
{
	//Daha önce sipariş olup olmaması kontrolü.
	if(jrb_empty(siparisler))
	{
		printf(ERROR "Sipariş yok\n");
		return 0;
	}

  	jrb_traverse(tmp, siparisler) {
		Siparis *siparis = (Siparis *) tmp->val.v;
		printf("%d, %s, %s, %s\n",siparis->sipKey, siparis->sipAd, siparis->sipMalzeme, siparis->sipRenk);
  	}
	return 1;
}

int Quit()
{
	//Ağaç temizleniyor.
	jrb_free_tree(siparisler);
	exit(0);
}

//verilen karakterler bir sayı mı kontrolü yapar.
int IsCharIsNumber(char number[])
{
	int i = 0;
	for (; number[i] != 0; i++)
	{
		if (number[i] > '9' || number[i] < '0')
			return 0;
	}
	return 1;
}

int main(int argc, char **argv, char **envp)
{
  	siparisler = make_jrb();
  	char satir[SATIRBUFFER];
	char *komutlar[KOMUTBUFFER];
	int komutSayisi;

	while(1)
	{
		//okunacak stdin resetlendi.
		memset(satir, '\0',SATIRBUFFER);
		//Klavyeden okuma
		if(isatty(STDIN_FILENO))
		{
			Prompt();
			fgets(satir, SATIRBUFFER, stdin);
		}
		//Dosyadan Okuma
		else
		{
			if (fgets(satir, SATIRBUFFER, stdin) == NULL)
			{
				//Dosya okunduysa stdin klavyeye geçiriliyor.
				freopen("/dev/tty", "rw", stdin);
				Prompt();
				fgets(satir, SATIRBUFFER, stdin);
			}
		}
		//Konsola girilen komut parse ediliyor.
		if((komutlar[0] = strtok(satir,"|\n")) == NULL) continue;
		komutSayisi = 1;
		while((komutlar[komutSayisi] = strtok(NULL, "|\n")) != NULL) 
		{
			komutSayisi++;
		}
		//Komutlar sarı renkte girliyordu. burada resetlendi.
		printf("\x1B[0m" );

		//Komutlar çözümleniyor.
		if (strcmp(komutlar[0], "add") == 0)
		{
			Add(komutlar);
		}
		else if(strcmp(komutlar[0], "search") == 0)
		{
			Search(komutlar);
		}
		else if(strcmp(komutlar[0], "write") == 0)
		{
			Write(komutlar);
		}
		else if(strcmp(komutlar[0], "print") == 0)
		{
			Print();
		}
		else if(strcmp(komutlar[0], "quit") == 0)
		{
			Quit();
		}
		else
		{
			printf("\x1B[31m" "ERROR: Bilinmeyen komut : %s\n" "\x1B[0m",komutlar[0]);
			return 0;
		}
	}
  return 0;
}

