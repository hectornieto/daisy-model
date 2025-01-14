using System;
using System.Runtime.InteropServices;

namespace CopenhagenUniversity.Life.OpenMI.Daisy
{

public class DaisyDotNetAccess
{
  [DllImport("daisy")] public static extern string 
  daisy_version ();
  
  [DllImport("daisy")] public static extern void 
  daisy_initialize ();
  
  [DllImport("daisy")] public static extern IntPtr
  daisy_syntax_create ();
  
  [DllImport("daisy")] public static extern IntPtr
  daisy_alist_create ();
  
  [DllImport("daisy")] public static extern void                           
  daisy_load (IntPtr syntax, IntPtr alist);
  
  [DllImport("daisy")] public static extern IntPtr
  daisy_parser_create_file (IntPtr syntax, string filename);
  
  [DllImport("daisy")] public static extern void                           
  daisy_parser_load (IntPtr parser, IntPtr alist);
  
  [DllImport("daisy")] public static extern bool 
  daisy_syntax_check (IntPtr syntax, IntPtr alist, string name);
  
  [DllImport("daisy")] public static extern uint 
  daisy_parser_error_count (IntPtr parser);

  [DllImport("daisy")] public static extern IntPtr
  daisy_daisy_create (IntPtr syntax, IntPtr alist);

  [DllImport("daisy")] public static extern bool 
  daisy_daisy_check (IntPtr daisy);

  [DllImport("daisy")] public static extern void                           
  daisy_syntax_delete (IntPtr syntax);

  [DllImport("daisy")] public static extern void                           
  daisy_alist_delete (IntPtr alist);

  [DllImport("daisy")] public static extern void                           
  daisy_parser_delete (IntPtr parser);

  [DllImport("daisy")] public static extern void                           
  daisy_daisy_delete (IntPtr daisy);

  [DllImport("daisy")] public static extern IntPtr
  daisy_daisy_get_time (IntPtr daisy);

  [DllImport("daisy")] public static extern uint         
  daisy_daisy_count_columns (IntPtr daisy);

  [DllImport("daisy")] public static extern void  
  daisy_daisy_start (IntPtr daisy);

  [DllImport("daisy")] public static extern IntPtr 	
  daisy_scope_find_extern (string name);

  [DllImport("daisy")] public static extern bool    
  daisy_daisy_is_running (IntPtr daisy);

  [DllImport("daisy")] public static extern void
  daisy_daisy_tick_action (IntPtr daisy);

  [DllImport("daisy")] public static extern void                      
  daisy_daisy_tick_weather (IntPtr daisy);

  [DllImport("daisy")] public static extern void    
  daisy_daisy_tick_column (IntPtr daisy, int col);

  [DllImport("daisy")] public static extern void
  daisy_daisy_tick_logs (IntPtr daisy);

  [DllImport("daisy")] public static extern void 
  daisy_daisy_tick_time (IntPtr daisy);

  [DllImport("daisy")] public static extern int
  daisy_time_get_hour (IntPtr daisy_time);

  [DllImport("daisy")] public static extern int
  daisy_time_get_mday (IntPtr daisy_time);

  [DllImport("daisy")] public static extern int
  daisy_time_get_month (IntPtr daisy_time);

  [DllImport("daisy")] public static extern int
  daisy_time_get_year (IntPtr daisy_time);

  [DllImport("daisy")] public static extern int
  daisy_scope_has_number (IntPtr scope, string name);

  [DllImport("daisy")] public static extern double 
  daisy_scope_number (IntPtr scope, string name);

  [DllImport("daisy")] public static extern string
  daisy_scope_dimension (IntPtr scope, string name);

  /* Declarations. */
  IntPtr syntax;
  IntPtr alist;
  IntPtr parser;
  IntPtr daisy;
  IntPtr scope;
  IntPtr daisy_time;

public void Initialize (string filename)
  {
    /* Local variable */
    syntax = (IntPtr) 0;
    alist = (IntPtr) 0;
    parser = (IntPtr) 0;
    daisy = (IntPtr) 0;
    scope = (IntPtr) 0;
    daisy_time = (IntPtr) 0;

    /* Link and initialize the daisy subsystem. */
    daisy_initialize ();

    /* Initialize syntax and attribute list. */
    syntax = daisy_syntax_create ();
    alist = daisy_alist_create ();
    daisy_load (syntax, alist);
    
    /* Parse the file. */
    parser = daisy_parser_create_file (syntax, filename);
    daisy_parser_load (parser, alist);
    
    /* Check the result. */
    if (!daisy_syntax_check (syntax, alist, "daisy")
	|| daisy_parser_error_count (parser) > 0)
      throw new ApplicationException ("Check failed");
    
    /* Create, check and run the simulation. */
    daisy = daisy_daisy_create (syntax, alist);
    if (!daisy_daisy_check (daisy))
      throw new ApplicationException ("Create failed");
  }

public void PerformTimeStep (uint columns)
  {
    daisy_daisy_tick_action (daisy);
    daisy_daisy_tick_weather (daisy);

    for (int i = 0; i < columns; i++)
      daisy_daisy_tick_column (daisy, i);

    daisy_daisy_tick_logs (daisy);
    daisy_daisy_tick_time (daisy);

    if (daisy_time_get_hour (daisy_time) == 0)
      {
	Console.Write ("*** " + daisy_time_get_year (daisy_time));
	Console.Write ("-" + daisy_time_get_month (daisy_time));
	Console.WriteLine ("-" + daisy_time_get_mday (daisy_time));
      }
  }
 
public void Finish ()
  {
    /* Cleanup. */
    daisy_syntax_delete (syntax);
    daisy_alist_delete (alist);
    daisy_parser_delete (parser);
    daisy_daisy_delete (daisy);
  }

public void RunSimulation (string filename) 
  {
    try 
      {
	/* Link and initialize the daisy subsystem. */
	Initialize (filename);

	/* Run the simulation. */
	{
	  daisy_time = daisy_daisy_get_time (daisy);
	  uint columns = daisy_daisy_count_columns (daisy);
	  
	  Console.WriteLine ("Starting simulation.");
	  daisy_daisy_start (daisy);
	    
	  //
	  scope = daisy_scope_find_extern ("check");
	  if (scope != (IntPtr) 0)
	    Console.WriteLine ("check OK end.");
	  else 
	    Console.WriteLine ("check not recognized.");
	  
	  while (daisy_daisy_is_running (daisy))
	    {
	      PerformTimeStep (columns);
	      if (scope != (IntPtr) 0)
		{
		  if(daisy_scope_has_number (scope, "height") == 1)
		    {
		      Console.Write ("Height"+ daisy_scope_number(scope, "height"));
		      Console.WriteLine (" " + daisy_scope_dimension (scope, "height"));
		    }
		  else  Console.WriteLine ("height not recognized.");
		}
	    }
	  Console.WriteLine ("Simulation end.");
	}
      }
    finally 
      {
	Finish();
      }
  }
}
}
