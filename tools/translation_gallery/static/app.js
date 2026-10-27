'use strict';
const $=id=>document.getElementById(id);let zoom=4,page=1,total=0,token='',timer,requestEpoch=0,renderedKey='';let lastData=null;const dirtyNotes=new Map();
function el(tag,text,cls){const n=document.createElement(tag);if(text!==undefined)n.textContent=text;if(cls)n.className=cls;return n;}
function metric(value,label){const n=el('div',undefined,'metric');n.append(el('strong',String(value)),el('small',label));return n;}
function picture(row,role,label){const pane=el('div',undefined,'pane');pane.append(el('h3',label));const canvas=el('div',undefined,'canvas');if(row[role+'_url']){const img=el('img');img.src=row[role+'_url'];img.alt=label+' — '+row.label;img.loading='lazy';img.width=row.dimensions[0]*zoom;img.height=row.dimensions[1]*zoom;img.dataset.nativeWidth=row.dimensions[0];img.dataset.nativeHeight=row.dimensions[1];img.addEventListener('error',()=>{img.remove();canvas.append(el('div','Image unavailable or hash changed. Reload the catalog.','placeholder'));});canvas.append(img);}else canvas.append(el('div',role==='output'?'Translation not created':'Source decoding or identification pending','placeholder'));pane.append(canvas);return pane;}
function key(){return JSON.stringify([$('query').value,$('status').value,$('batch').value,page]);}
function params(){const view=$('status').value;const draft=['translated','unreviewed','reviewed','approve','request_changes','stale'].includes(view);return new URLSearchParams({q:$('query').value,kind:draft?'draft':'',status:['translated','inventory'].includes(view)?'':view,batch:$('batch').value,page:String(page)});}
function needsReview(style){return ['unreviewed','stale'].includes(style);}
function inView(style,view){if(view==='unreviewed')return needsReview(style);if(view==='reviewed')return ['approve','request_changes','defer'].includes(style);if(view==='translated'||view==='inventory')return true;return style===view;}
function counters(data){const s=data.summary;$('summary').replaceChildren(metric(s.drafts,'translated drafts'),metric(s.style_approved,'style approved'),metric(s.changes_requested,'changes requested'),metric(s.mapped_japanese+' / '+s.known_japanese,'Japanese regions mapped'),metric(s.suspected,'suspected text'),metric(s.probable_images_unrepresented,'probable images not decoded'));$('result-count').textContent=`${total.toLocaleString()} results · ${s.decoded_regions.toLocaleString()} decoded regions inventoried`;$('page').textContent=`${page} / ${Math.max(1,Math.ceil(total/24))}`;$('previous').disabled=page<=1;$('next').disabled=page*24>=total;}
function emptyState(){const panel=el('div',undefined,'card');const all=$('status').value==='unreviewed'&&!$('query').value&&!$('batch').value;panel.append(el('h2',all?'All current drafts reviewed':'No images match these filters'),el('p',all?'New batches will appear in Needs review. Your saved approvals and comments are in Reviewed.':'Try a different search or review view.','quiet'));if(all){const reviewed=el('button','View reviewed','primary');reviewed.onclick=()=>{$('status').value='reviewed';page=1;load();};const refresh=el('button','Refresh');refresh.onclick=()=>load();panel.append(reviewed,document.createTextNode(' '),refresh);}return panel;}
function removeAnchored(c){const anchor=c.nextElementSibling||c.previousElementSibling;const top=anchor?anchor.getBoundingClientRect().top:0;c.remove();if(anchor)requestAnimationFrame(()=>{if(anchor.isConnected){window.scrollBy(0,anchor.getBoundingClientRect().top-top);const focus=anchor.querySelector('textarea,button');if(focus)focus.focus({preventScroll:true});}});}
function card(row){
 const c=el('article',undefined,'card');c.dataset.assetId=row.id;
 const head=el('div',undefined,'card-head'),left=el('div');left.append(el('h2',row.label),el('div',`${row.source_container} · Frame ${row.frame??'unknown'} · Palette ${row.palette??'unknown'} · ${row.variant??'unknown'}${row.dimensions?' · '+row.dimensions.join('×'):''}`,'meta'));
 const badge=el('span',row.style==='approve'?'Style approved':row.status.replaceAll('_',' '),'badge '+row.status);head.append(left,badge);c.append(head);
 const pair=el('div',undefined,'pair');pair.append(picture(row,'original','Japanese original'),picture(row,'output','English translation'));c.append(pair,el('div',(row.context||[]).join(' · ')+(row.kind==='draft'?' · Mapping: '+row.mapping:''),'annotation'));
 if(row.image_error)c.append(el('div',row.image_error,'annotation'));
 if(row.kind!=='draft'){c.append(el('div',row.reason||'Original retained in the inventory; no translation is available to approve.','annotation'));return c;}
 const noteKey=row.id+':'+row.revision,review=el('div',undefined,'review'),label=el('label','Review notes'),notes=el('textarea');notes.value=dirtyNotes.has(noteKey)?dirtyNotes.get(noteKey):(row.notes||'');notes.maxLength=4000;notes.placeholder='Comment on this exact image version';notes.oninput=()=>dirtyNotes.set(noteKey,notes.value);label.append(notes);review.append(label);
 const actions=el('div',undefined,'actions'),feedback=el('div','','feedback');
 for(const [decision,text,cls] of [['approve','Approve style','primary'],['request_changes','Request changes',''],['defer','Save comment','']]){
  const b=el('button',text,cls);b.disabled=!row.reviewable;
  b.onclick=async()=>{
   if(decision==='defer'&&!notes.value.trim()){feedback.textContent='Add a comment first.';notes.focus();return;}
   const buttons=[...actions.querySelectorAll('button')],savedNotes=notes.value,saveKey=key();buttons.forEach(x=>x.disabled=true);notes.disabled=true;feedback.textContent='Saving…';
   try{
    const response=await fetch('/api/reviews',{method:'POST',headers:{'Content-Type':'application/json','X-Review-Token':token},body:JSON.stringify({asset_id:row.id,revision:row.revision,decision,notes:savedNotes})});const result=await response.json();if(!response.ok)throw Error(result.error||'Review not saved');
    requestEpoch++;if(dirtyNotes.get(noteKey)===savedNotes)dirtyNotes.delete(noteKey);const oldStyle=row.style;row.style=result.decision;row.status=result.decision;row.notes=savedNotes;badge.textContent=decision==='approve'?'Style approved':decision.replaceAll('_',' ');badge.className='badge '+decision;feedback.textContent='Saved locally';
    if(lastData){lastData.summary.style_approved+=Number(decision==='approve')-Number(oldStyle==='approve');lastData.summary.changes_requested+=Number(decision==='request_changes')-Number(oldStyle==='request_changes');}
    if(c.isConnected&&saveKey===key()&&renderedKey===saveKey){
     if(!inView(decision,$('status').value)){total=Math.max(0,total-1);removeAnchored(c);}
     if(lastData)counters(lastData);
     // Preserve every remaining card and its textarea. Fetch only once this page is empty.
     if(!$('assets').querySelector('article')){page=Math.max(1,Math.min(page,Math.ceil(total/24)));await load();}
    }else if(saveKey!==key()||renderedKey!==key()){await load();}
   }catch(e){feedback.textContent=e.message;}
   finally{notes.disabled=false;buttons.forEach(x=>x.disabled=!row.reviewable);}
  };
  actions.append(b);
 }
 review.append(actions);c.append(review,feedback);return c;
}
async function load(){
 const epoch=++requestEpoch,requestedKey=key();
 try{
  const response=await fetch('/api/catalog?'+params());const data=await response.json();if(epoch!==requestEpoch||requestedKey!==key())return;if(!response.ok)throw Error(data.error||'Catalog unavailable');
  if(data.total>0&&data.items.length===0&&page>1){page=Math.max(1,Math.ceil(data.total/24));return load();}
  token=data.review_token;total=data.total;lastData=data;renderedKey=key();$('error').hidden=true;counters(data);
  const batchValue=$('batch').value;$('batch').replaceChildren(new Option('All batches',''),...data.batches.map(b=>new Option(`${b.id} · ${b.style_approved}/${b.drafts} style approved`,b.id)));$('batch').value=batchValue;
  $('assets').replaceChildren(...(data.items.length?data.items.map(card):[emptyState()]));
  const s=data.summary;$('coverage').textContent=`Coverage remains open: ${s.unrepresented_sources.toLocaleString()} unrepresented sources (${s.probable_images_unrepresented} probable images), ${s.unresolved_graphics_sources} unresolved graphics sources. Counts overlap; unknown backlog is not a completion percentage. Style approval is hash-bound and does not accept technical or runtime behavior.`;
 }catch(e){if(epoch===requestEpoch){$('error').hidden=false;$('error').textContent=e.message;}}
}
$('query').addEventListener('input',()=>{clearTimeout(timer);timer=setTimeout(()=>{page=1;load();},250);});
for(const id of ['status','batch'])$(id).addEventListener('change',()=>{page=1;load();});
$('previous').onclick=()=>{page--;load();window.scrollTo(0,0);};$('next').onclick=()=>{page++;load();window.scrollTo(0,0);};
for(const b of document.querySelectorAll('[data-zoom]'))b.onclick=()=>{zoom=Number(b.dataset.zoom);for(const x of document.querySelectorAll('[data-zoom]'))x.classList.toggle('active',x===b);for(const img of document.querySelectorAll('.canvas img')){img.width=Number(img.dataset.nativeWidth)*zoom;img.height=Number(img.dataset.nativeHeight)*zoom;}};
load();
