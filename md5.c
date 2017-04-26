#include<stdio.h>
#include <string.h> 
#include <stdlib.h>

int main()
{    
    char linha[512];
	char comando[sizeof(linha)];
	char fileHash[33],name[sizeof(linha)];
	char actHash[33],actNameDir[sizeof(linha)];

	FILE *tempArq = fopen("activetemp.txt", "w");
    FILE *arqMonitorado = fopen("requested.txt", "r");
	while((fscanf(arqMonitorado,"%s", linha))!=EOF)
	{
		int actInt = 0;
		strcpy(comando,"md5sum ");	
		strcat(comando,linha);

		FILE *fp = popen(comando, "r");
		while ((fscanf(fp,"%s  %s", fileHash, name))!=EOF)
		pclose(fp);

		FILE *fimlog = fopen("fimlog.txt", "a+");
		if (fileHash != NULL)
		{
			FILE *actFile = fopen("activefile.txt", "r");			
			while((fscanf(actFile,"%s = %s", actHash, actNameDir))!=EOF)
			{

				if ((strcmp(actNameDir,name) == 0) && (strcmp(actHash,fileHash) == 0))
				{
					actInt = 1; 
					fprintf(tempArq,"%s = %s\n", fileHash, name);	
				}
				if ((strcmp(actNameDir,name) == 0) && (strcmp(actHash,fileHash) != 0))
				{
					actInt = 1;
					fprintf(tempArq,"%s = %s\n", fileHash, name);
					fprintf(fimlog," FIM Log = O arquivo %s foi modificado para o MD5 = %s\n", name, fileHash);
				}
			}
			fclose(actFile);
			if (actInt == 0)
			{
				fprintf(fimlog," FIM Log = O arquivo %s foi criado com o MD5 = %s\n", name, fileHash);
				fprintf(tempArq,"%s = %s\n",fileHash, name);
			}
		}
		fclose(fimlog);
	}	
    fclose(arqMonitorado);
	fclose(tempArq);
	system("cp activetemp.txt activefile.txt");
	system("rm activetemp.txt");
	return 0;
}
