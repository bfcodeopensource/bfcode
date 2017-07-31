#include <iostream>
#include <stdio.h>
#include <stdlib.h>

#include "BF.h"
#include "CBF.h"

#define _BF
#define _CBF


#define DATA_SIZE 1000000 // 1M

#define UNIT 100000 // 0.1M


#ifdef _BF
ull items_bf[4 * DATA_SIZE + 10];

void test_bf(int n, int k, int m, BF* bf, FILE * f_res)
{
	bf->init(m, k);

	bf->insert_all(items_bf, n);

	double emprical_fp = bf->query_fp(items_bf + DATA_SIZE, DATA_SIZE * 3);
	double bloom_fp = bf->query_fp_bloom();
	double bose_up = bf->query_up_bose();
	double ours_up = bf->query_up_ours();

	double opt_k_bloom = bf->query_opt_k_bloom();
	double opt_k_ours = bf->query_opt_k_ours();


	fprintf(f_res, "%d\t%d\t%8d\t\t%.4lf\t%.4lf\t%.4lf\t%.4lf\t%.4lf\t%.4lf\t%.4lf\t%.4lf\n", 
		n, k, m, emprical_fp, bloom_fp, bose_up, ours_up, (bose_up - bloom_fp) / bloom_fp, (ours_up - bloom_fp) / bloom_fp, opt_k_bloom, opt_k_ours);

}

void bf_exp()
{

	FILE * pf = fopen("distinctIP_sample.dat", "rb");

	char buf[2048];
	int item_cnt = 0;
	while (fread(buf, 1, 8, pf))
	{
		items_bf[item_cnt] = *((ull*)buf);
		item_cnt ++;
	}

	BF * bf = new BF();




	FILE *f_res;
	printf("********************************************** BF test *********************************************\n");
	printf("n\tk\t m\t\temprical_fp\tbloom_fp\tbose_up\tours_up\tbounds_error_ratio_of_Bose\tbounds_error_ratio_of_ours\tbose_opt_k\tour_opt_k\n\n");

	printf("bf test-1 (varying n) starts!\n");
	// f_res = fopen("upbound_n.raw.dat", "w");
	for(int n = UNIT; n <= 10 * UNIT; n += UNIT)
	{
		test_bf(n, 6, 50 * UNIT, bf, stdout);
		bf->clear();
	}
	// fclose(f_res);	
	printf("bf test-1 (varying n) ends!\n\n");


	printf("bf test-2 (varying k) starts!\n");
	// f_res = fopen("upbound_k.raw.dat", "w");
	for(int k = 3; k <= 12; k++)
	{
		test_bf(5 * UNIT, k, 50 * UNIT, bf, stdout);
		bf->clear();
	}
	// fclose(f_res);
	printf("bf test-2 (varying k) ends!\n\n");


	printf("bf test-3 (varying m) starts!\n");
	// f_res = fopen("upbound_m.raw.dat", "w");
	for(int m = 10 * UNIT; m <= 100 * UNIT; m += 10 * UNIT)
	{
		test_bf(5 * UNIT, 6, m, bf, stdout);
		bf->clear();
	}
	// fclose(f_res);
	printf("bf test-3 (varying m) ends!\n\n");
	printf("****************************************************************************************************\n\n");




}

#endif //_BF

#ifdef _CBF
ull items_cbf[DATA_SIZE + 10];
int items_fs[DATA_SIZE + 10];

void test_cbf(int n, int k, int m, CBF* cbf, FILE * f_res)
{
	cbf->init(m, k);

	cbf->insert_all(items_cbf, items_fs, n);

	double emprical_cr = cbf->query_cr(items_cbf, items_fs, n);
	double upper_cr = cbf->query_cr_upper();
	double lower_cr = cbf->query_cr_lower();


	fprintf(f_res, "%d\t%d\t%8d\t\t%.4lf\t%.4lf\t%.4lf\n", n, k, m, emprical_cr, upper_cr, lower_cr);

}

void cbf_exp()
{	
	FILE * pf = fopen("IP_sample.dat", "rb");

	char buf[2048];
	int item_cnt = 0;

	while (fread(buf, 1, 12, pf))
	{
		items_cbf[item_cnt] = *((ull*)buf);
		items_fs[item_cnt] = *((int*)buf + 2);

		item_cnt ++;
	}


	CBF * cbf = new CBF();

	FILE *f_res;
	printf("********************************************** CBF test *********************************************\n");
	printf("n\tk\t m\t\temprical_cr\tupper_cr\tlower_cr\n\n");


	printf("cbf test-1 (varying n) starts!\n");
	// f_res = fopen("cr_n.raw.dat", "w");
	for(int n = UNIT; n <= 10 * UNIT; n += UNIT)
	{
		test_cbf(n, 6, 50 * UNIT, cbf, stdout);
		cbf->clear();
	}
	// fclose(f_res);	
	printf("cbf test-1 (varying n) ends!\n\n");
 

	printf("cbf test-2 (varying k) starts!\n");
	// f_res = fopen("cr_k.raw.dat", "w");
	for(int k = 3; k <= 12; k++)
	{
		test_cbf(5 * UNIT, k, 50 * UNIT, cbf, stdout);
		cbf->clear();
	}
	// fclose(f_res);
	printf("cbf test-2 (varying k) ends!\n\n");

	printf("cbf test-3 (varying m) starts!\n");
	// f_res = fopen("cr_m.raw.dat", "w");
	for(int m = 10 * UNIT; m <= 100 * UNIT; m += 10 * UNIT)
	{
		test_cbf(5 * UNIT, 6, m, cbf, stdout);
		cbf->clear();
	}
	// fclose(f_res);
	printf("cbf test-3 (varying m) ends!\n\n");

	printf("*****************************************************************************************************\n");

}
#endif //_CBF





int main(int argc, char ** argv)
{

#ifdef _BF
	bf_exp();
#endif //_BF

#ifdef _CBF
	cbf_exp();
#endif //_CBF

	return 0;
}