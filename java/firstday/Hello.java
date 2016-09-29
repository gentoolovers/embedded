class Mobile
{
    String Brand = "samsung";
    void setBrand(String s)
        {
            Brand=s;
        }
}

public class Hello
{
    public static void main(String[] args)
        {

            Mobile mob=new Mobile();
            System.out.println("hello Beijing " + mob.Brand);
            mob.setBrand("我来了");
            System.out.println("hello Beijing " + mob.Brand);
        }
}
