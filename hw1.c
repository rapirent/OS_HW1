#include <linux/init.h>
#include <linux/module.h>
#include <linux/moduleparam.h>
#include <linux/kernel.h>
#include <linux/string.h>

MODULE_LICENSE("Dual BSD/GPL");
MODULE_AUTHOR("KuoTeng, Ding, E94036209");
MODULE_DESCRIPTION("OS_hw1");
MODULE_VERSION("1.0.0");
//declare defualt parameter
static int mask = 111;
static char* name1 = "swap_string";
static char* name2 = "calc";
static char* name3 = "sum_tree";
//kernel module
static struct kobject *hw1_kobject;
//declare callback function and module attribute
static ssize_t swap_string_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t swap_string_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static struct kobj_attribute swap_string_attribute = __ATTR(swap_string, 0660, swap_string_show, swap_string_store);

static ssize_t calc_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t calc_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static struct kobj_attribute calc_attribute = __ATTR(calc, 0660, calc_show, calc_store);

static ssize_t sum_tree_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf);
static ssize_t sum_tree_store(struct kobject *kobj, struct kobj_attribute *attr,const char *buf, size_t count);
static struct kobj_attribute sum_tree_attribute = __ATTR(sum_tree, 0660, sum_tree_show, sum_tree_store);

//the module parameter
module_param(mask, int , S_IRWXU);
module_param(name1, charp, S_IRWXU);
module_param(name2, charp, S_IRWXU);
module_param(name3, charp, S_IRWXU);

//just some necessaryfunction parameter
static char new_string[100]={'\0'};
static int cal_shu = 0;
static char print_string[100]={'\0'};

static ssize_t swap_string_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", new_string);
}
static ssize_t swap_string_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
	char origin_string[100] = {'\0'};
	int string_num;
	int origin_strlen;
	int foo,bar;
    for(foo = 0; foo < 100; foo++)
    {
        new_string[foo]='\0';
    }
	sscanf(buf, "%d %s",& string_num, origin_string);
	origin_strlen = strlen(origin_string);
    new_string[origin_strlen] = '\0';
	for(foo = string_num, bar = 0; foo< origin_strlen ; foo++)
	{
		new_string[bar++] = origin_string[foo];
		origin_string[foo] = '\0';
	}
    strncat(new_string,origin_string,string_num);;
	return count;
}
static ssize_t calc_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%d\n" , cal_shu);
}
static ssize_t calc_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    char input[100] = {'\0'};
    int parser[100] = {0};
    int distinguish[100] = {0};
    int parser_top=0;
    int foo,bar;
    int stop = 0;
    int operand_1,operand_2,operator;
    int operand_1_ind=0,operand_2_ind=0;
    int order = 0;
    int statu = 0;
    strcpy(input, buf);
    cal_shu = 0;
    for(foo = 0,parser_top = 0, order = 0; foo < strlen(input); foo++)
    {
        //shu zhi
        if(input[foo]!=' ')
        {
            if(order == 0)
            {
                distinguish[parser_top] = 1;// operand
                sscanf((input + foo) ,"%d", &parser[parser_top++]);
                for(stop = 0;stop != 1;)
                {
                    switch(input[foo])
                    {
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                        case '0':
                        case '+':
                        case '-':
                            foo++;
                         break;
                        default:
                            stop = 1;
                            break;
                    }
                }
                order = 1;
            }
            else
            {
                distinguish[parser_top] = -1;//operator
                switch(input[foo])
                {
                    case '+':
                        parser[parser_top++] = -1;
                        break;
                    case '-':
                        parser[parser_top++] = -2;
                        break;
                    case '*':
                        parser[parser_top++] = -3;
                        break;
                    case '/':
                        parser[parser_top++] = -4;
                        break;
                    case '%':
                        parser[parser_top++] = -5;
                        break;
                }
                order = 0;
            }
        }
    }
    for(foo = 0;foo<parser_top;foo++)
    {
        if(distinguish[foo]==-1 &&(parser[foo]==-3||parser[foo]==-4||parser[foo]==-5))//operator
        {
            operand_1 = operand_2 = 0;
            for(bar = foo;bar >=0;bar--)
            {
                if(distinguish[bar]==1)//operand
                {
                    operand_1 = parser[bar];
                    operand_1_ind = bar;
                    break;
                }
            }
            for(bar = foo; bar < parser_top ; bar++)
            {
                if(distinguish[bar]==1)
                {
                    operand_2 = parser[bar];
                    operand_2_ind = bar;
                    break;
                }
            }
            distinguish[operand_1_ind] = 1;//give result to first operand
            distinguish[foo] = 0;
            distinguish[operand_2_ind] = 0;
            switch(parser[foo])
            {
                // case -1:
                //     operator =
                //     break;
                // case -2:
                //     parser[foo+1] = operan_1 - operand_2;
                //     break;
                case -3:
                    parser[operand_1_ind] = operand_1 * operand_2;//give result to first operand
                    parser[foo] = 0;
                    parser[operand_2_ind] = 0;
                    break;
                case -4:
                    parser[operand_1_ind] = operand_1 / operand_2;//give result to first operand
                    parser[foo] = 0;
                    parser[operand_2_ind] = 0;
                    break;
                case -5:
                    parser[operand_1_ind] = operand_1 % operand_2;//give result to first operand
                    parser[foo] = 0;
                    parser[operand_2_ind] = 0;
                    break;
                default:
                    break;
            }
            order = 0;
        }
    }
    for(foo = 0;foo<parser_top;foo++)
    {
        if(distinguish[foo]==1)//operand
        {
            if(statu==0)//no operand
            {
                cal_shu += parser[foo];
            }
            if(statu==1)
            {
                operand_2 = parser[foo];
                switch(operator)
                {
                    case -1:
                        cal_shu+=operand_2;
                        break;
                    case -2:
                        cal_shu-=operand_2;
                        break;
                }
                statu = 0;
            }
        }
        if(distinguish[foo]==-1)//operator
        {
            operator = parser[foo];
            statu=1;
        }
    }
	return count;
}

static ssize_t sum_tree_show(struct kobject *kobj, struct kobj_attribute *attr, char *buf)
{
	return sprintf(buf, "%s\n", print_string);
}
static ssize_t sum_tree_store(struct kobject *kobj, struct kobj_attribute *attr, const char *buf, size_t count)
{
    char input[100]={'\0'};
    char output[50]={'\0'};
    int foo,bar;
    int stack[100]={0};
    int stack_top = 0;
    int stop = 0;
    int num = 0;
    int store_stack = 0;//if it just store a shu to stack
    int first=0;
    strcpy(input, buf);
    for(foo = 0; foo < 100; foo++)
    {
        print_string[foo]='\0';
    }
    for(foo = 0; foo < strlen(input); foo++)
    {
        switch(input[foo])
        {
            case '(':
                break;
            case ')':
            case ' ':
                if(stack_top >= 1 && store_stack == 1)
                {
                    for(bar=0;bar<stack_top;bar++)
                    {
                        num+=stack[bar];
                    }
                    if(first == 1)
                    {
                        strcat(print_string , ", ");
                    }
                    sprintf(output, "%d" , num);
                    strcat(print_string , output);
                    first = 1;
                    num = 0;
                    stack[stack_top--]=0;
                }
                if(input[foo]==')')
                {
                    stack[stack_top--]=0;
                }
                store_stack = 0;
                break;
            case '0':
            case '1':
            case '2':
            case '3':
            case '4':
            case '5':
            case '6':
            case '7':
            case '8':
            case '9':
            case '+':
            case '-':
                store_stack = 1;
                sscanf((input + foo) ,"%d", &stack[stack_top++]);
                for(stop = 0;stop != 1;)
                {
                    switch(input[foo])
                    {
                        case '1':
                        case '2':
                        case '3':
                        case '4':
                        case '5':
                        case '6':
                        case '7':
                        case '8':
                        case '9':
                        case '0':
                        case '+':
                        case '-':
                            foo++;
                            break;
                        default:
                            stop = 1;
                            break;
                    }
                }
                foo--;
                break;
            default:
                // printf("WTF!!!!");
                break;
        }
    }
	return count;
}
static int __init iniitialize(void)
{
	hw1_kobject = kobject_create_and_add("hw1", kernel_kobj);
	swap_string_attribute.attr.name = name1;
	calc_attribute.attr.name = name2;
	sum_tree_attribute.attr.name = name3;
	switch(mask)
	{
		case 8:
			//010
			sysfs_create_file(hw1_kobject, &calc_attribute.attr);
			break;
		case 9:
			//011
			sysfs_create_file(hw1_kobject, &calc_attribute.attr);
			sysfs_create_file(hw1_kobject, &sum_tree_attribute.attr);
			break;
		case 0:
			//000
			break;
		case 1:
			//001
			sysfs_create_file(hw1_kobject, &sum_tree_attribute.attr);
			break;
		case 100:
			sysfs_create_file(hw1_kobject, &swap_string_attribute.attr);
			break;
		case 110:
			sysfs_create_file(hw1_kobject, &swap_string_attribute.attr);
			sysfs_create_file(hw1_kobject, &calc_attribute.attr);
			break;
		case 111:
			sysfs_create_file(hw1_kobject, &swap_string_attribute.attr);
			sysfs_create_file(hw1_kobject, &calc_attribute.attr);
			sysfs_create_file(hw1_kobject, &sum_tree_attribute.attr);
			break;
		case 101:
			sysfs_create_file(hw1_kobject, &swap_string_attribute.attr);
			sysfs_create_file(hw1_kobject, &sum_tree_attribute.attr);
			break;
	}
	return 0;
}

static void __exit cleanup(void)
{
	switch(mask)
	{
		case 8:
			//010
			sysfs_remove_file(hw1_kobject, &calc_attribute.attr);
			break;
		case 9:
			//011
			sysfs_remove_file(hw1_kobject, &calc_attribute.attr);
			sysfs_remove_file(hw1_kobject, &sum_tree_attribute.attr);
			break;
		case 0:
			//000
			break;
		case 1:
			//001
			sysfs_remove_file(hw1_kobject, &sum_tree_attribute.attr);
			break;
		case 100:
			sysfs_remove_file(hw1_kobject, &swap_string_attribute.attr);
			break;
		case 110:
			sysfs_remove_file(hw1_kobject, &swap_string_attribute.attr);
			sysfs_remove_file(hw1_kobject, &calc_attribute.attr);
			break;
		case 111:
			sysfs_remove_file(hw1_kobject, &swap_string_attribute.attr);
			sysfs_remove_file(hw1_kobject, &calc_attribute.attr);
			sysfs_remove_file(hw1_kobject, &sum_tree_attribute.attr);
			break;
		case 101:
			sysfs_remove_file(hw1_kobject, &swap_string_attribute.attr);
			sysfs_remove_file(hw1_kobject, &sum_tree_attribute.attr);
			break;
	}
	if(hw1_kobject){
		kobject_del(hw1_kobject);
	}
}

module_init(iniitialize);
module_exit(cleanup);
